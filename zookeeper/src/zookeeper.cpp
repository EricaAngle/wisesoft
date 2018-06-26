#include "zookeeper.hpp"
#include <cassert>
#include <cerrno>
#include <cstring>
//#include <mutex>
#include "zookeeper_error.hpp"
#include "zk_adaptor.h"

namespace zk
{

#define CHECK_ZOOCODE_AND_THROW(code)  \
	if (code != ZOK) { throw ZooException(code); }

void ZooKeeper::GlobalWatchFunc(zhandle_t *h, int type, int state, const char *path, void *ctx)
{
	ZooKeeper* self = static_cast<ZooKeeper *>(ctx);
	self->WatchHandler(type, state, path);
}

//static std::once_flag ONCE_FLAG_SET_DEBUG_LEVEL;
void set_default_debug_level()
{
    static int i = 1; 
	if( i ==1 ){
		zoo_set_debug_level(ZOO_LOG_LEVEL_WARN);
		++i;
    }	 
}

ZooKeeper::ZooKeeper(const ZooKeeperConf &conf)
{
	zoo_handle_ = zookeeper_init(conf.connectStr().c_str(), GlobalWatchFunc, conf.timeout, _client_conect_id, this, 0);

	if (zoo_handle_)
	{
		_client_conect_id = &(zoo_handle_->client_id);
	}
	else
	{
		_client_conect_id = NULL;
		throw ZooSystemErrorFromErrno(errno);
	}

	/*
	char buffer[512];
	char scheme[10] = "digest";
	char usr_pa[100] = "test:V28q/NynI4JI3Rk54h0r8O5kMug=";
	struct ACL_vector aclv;
	aclv.count = 1;
	struct ACL acl;
	acl.perms = ZOO_PERM_ALL;
	acl.id.scheme = scheme;
	acl.id.id = usr_pa;
	aclv.data = &acl;
	zoo_create(zk, "/mynode", "cluster", strlen("cluster"), &conf.aclv, 0, buffer, sizeof(buffer) - 1);

	zoo_add_auth(zk, "digest", "test:test", 7, NULL, NULL);

	memset(buffer, 0, sizeof(buffer));
	int buflen = sizeof(buffer);
	struct Stat stat;
	int rc = zoo_get(zk, "/mynode", 0, buffer, &buflen, &stat);

	if (rc)
	{
		fprintf(stderr, "zoo_get Error %d for %d\n", rc, __LINE__);
	}

	printf("get buffer: %s\n", buffer);

	rc = zoo_delete(zk, "/mynode", -1);

	if (rc)
	{
		fprintf(stderr, "zoo_delete Error %d for %d\n", rc, __LINE__);
	}

	sleep(1000000);
	*/
}

int
ZooKeeper::add_Auth(const std::string &scheme/*digest*/, const std::string &username, const std::string &password)
{
	std::stringbuf buffer;
	std::iostream outstream(&buffer);
	outstream << username << ":" << password;
	std::string user_passwd = buffer.str();
	return zoo_add_auth(zoo_handle_, scheme.c_str(), user_passwd.c_str(), user_passwd.size(), NULL, NULL);
}

ZooKeeper::ZooKeeper(){

}

ZooKeeper::ZooKeeper(const std::string &server_hosts,
					 ZooWatcher *global_watcher,
					 int timeout_ms)
	: zoo_handle_(NULL), _client_conect_id(NULL),
    global_watcher_(global_watcher)
{
	set_default_debug_level();

	zoo_handle_ = zookeeper_init(server_hosts.c_str(),
								 GlobalWatchFunc,
								 timeout_ms,
								 NULL, // client id
								 this,
								 0);

	if (!zoo_handle_)
	{
		throw ZooSystemErrorFromErrno(errno);
	}
}

ZooKeeper::~ZooKeeper()
{
	if (zoo_handle_)
	{
		int ret = zookeeper_close(zoo_handle_);

		if (ret != ZOK)
		{
			// TODO: log information
		}
	}
}

bool ZooKeeper::connectZK(const std::string &server_hosts,
						  ZooWatcher *global_watcher,
						  int timeout_ms, int max_try_time = 5)
{
	 global_watcher_=global_watcher;
	set_default_debug_level(); 
	do
	{
		zoo_handle_ = zookeeper_init(server_hosts.c_str(),
			GlobalWatchFunc,
			timeout_ms,
			NULL, // client id
			this,
			0);
		if (zoo_handle_)
			return true; 
		--max_try_time;
	}
	while (max_try_time > 0);

	if (!zoo_handle_) {
		throw ZooSystemErrorFromErrno(errno); 
	}
	return false;
}

bool ZooKeeper::is_connected()
{
	return zoo_state(zoo_handle_) == ZOO_CONNECTED_STATE;
}

bool ZooKeeper::is_expired()
{
	return zoo_state(zoo_handle_) == ZOO_EXPIRED_SESSION_STATE;
}

void ZooKeeper::WatchHandler(int type, int state, const char *path)
{
	// call global watcher
	if (!global_watcher_) return;

	if (type == ZOO_SESSION_EVENT)
	{
		if (state == ZOO_EXPIRED_SESSION_STATE)
		{
			global_watcher_->OnSessionExpired();
		}
		else if (state == ZOO_CONNECTED_STATE)
		{
			global_watcher_->OnConnected();
		}
		else if (state == ZOO_CONNECTING_STATE)
		{
			global_watcher_->OnConnecting();
		}
		else
		{
			// TODO:
			assert(0 && "don't know how to process other session event yet");
		}
	}
	else if (type == ZOO_CREATED_EVENT)
	{
		global_watcher_->OnCreated(path);
	}
	else if (type == ZOO_DELETED_EVENT)
	{
		global_watcher_->OnDeleted(path);
	}
	else if (type == ZOO_CHANGED_EVENT)
	{
		global_watcher_->OnChanged(path);
	}
	else if (type == ZOO_CHILD_EVENT)
	{
		global_watcher_->OnChildChanged(path);
	}
	else if (type == ZOO_NOTWATCHING_EVENT)
	{
		global_watcher_->OnNotWatching(path);
	}
	else
	{
		assert(false && "unknown zookeeper event type");
	}
}

bool ZooKeeper::Exists(const std::string &path, bool watch, NodeStat *stat)
{
	int zoo_code = zoo_exists(zoo_handle_, path.c_str(), watch, stat);

	if (zoo_code == ZNONODE)
	{
		return false;
	}
	else
	{
		CHECK_ZOOCODE_AND_THROW(zoo_code);
		return true;
	}
}

NodeStat ZooKeeper::Stat(const std::string &path)
{
	NodeStat stat;
	int zoo_code = zoo_exists(zoo_handle_, path.c_str(), false, &stat);
	CHECK_ZOOCODE_AND_THROW(zoo_code);

	return stat;
}

std::string ZooKeeper::Create(const std::string &path, const std::string &value, int flag)
{
	return Create(path, value, ZOO_OPEN_ACL_UNSAFE, flag);
}

/**
flag: allowed value  ZOO_EPHEMERAL
eg: flag: ZOO_EPHEMERAL| 
*/
std::string
ZooKeeper::Create(const std::string &path, const std::string &value, struct ACL_vector &acl, int flag /* = 0 */)
{
	std::string path_buffer;
	path_buffer.resize(path.size() + 64);

	int zoo_code = zoo_create(zoo_handle_,
							   path.c_str(),
							   value.data(),
							   value.size(),
							   &acl,
							   flag,
							   const_cast<char *>(path_buffer.data()),
							   path_buffer.size());

	CHECK_ZOOCODE_AND_THROW(zoo_code);

	path_buffer.resize(strlen(path_buffer.data()));
	return path_buffer;
}

std::string ZooKeeper::CreateIfNotExists(const std::string &path, const std::string &value, int flag)
{
	std::string path_buffer;
	path_buffer.resize(path.size() + 64);

	int zoo_code = zoo_create(zoo_handle_,
							   path.c_str(),
							   value.data(),
							   value.size(),
							   &ZOO_OPEN_ACL_UNSAFE,
							   flag,
							   const_cast<char *>(path_buffer.data()),
							   path_buffer.size());

	if (zoo_code == ZNODEEXISTS)
	{
		assert(!(flag & ZOO_SEQUENCE));
		return path;
	}

	CHECK_ZOOCODE_AND_THROW(zoo_code);

	path_buffer.resize(strlen(path_buffer.data()));
	return path_buffer;
}

void ZooKeeper::Delete(const std::string &path)
{
	NodeStat stat;

	if (!Exists(path.c_str(), false, &stat))
	{
		throw ZooException(ZNONODE);
	}

	int zoo_code = zoo_delete(zoo_handle_, path.c_str(), stat.version);
	CHECK_ZOOCODE_AND_THROW(zoo_code);
}

void ZooKeeper::DeleteIfExists(const std::string &path)
{
	NodeStat stat;

	if (!Exists(path.c_str(), false, &stat))
	{
		return;
	}

	int zoo_code = zoo_delete(zoo_handle_, path.c_str(), stat.version);

	if (zoo_code == ZNONODE)
	{
		return;
	}

	CHECK_ZOOCODE_AND_THROW(zoo_code);
}

std::string ZooKeeper::Get(const std::string &path, bool watch)
{
	std::string value_buffer;

	zk::NodeStat node_stat = Stat(path);

	value_buffer.resize(node_stat.dataLength);

	int buffer_len = value_buffer.size();
	int zoo_code = zoo_get(zoo_handle_,
							path.c_str(),
							watch,
							const_cast<char *>(value_buffer.data()),
							&buffer_len,
							&node_stat);

	CHECK_ZOOCODE_AND_THROW(zoo_code);

	value_buffer.resize(buffer_len);
	return value_buffer;
}

void ZooKeeper::Set(const std::string &path, const std::string &value)
{
	zk::NodeStat node_stat = Stat(path);

	int zoo_code = zoo_set(zoo_handle_,
							path.c_str(),
							value.data(),
							value.size(),
							node_stat.version);

	CHECK_ZOOCODE_AND_THROW(zoo_code);
}

std::vector<std::string> ZooKeeper::GetChildren(const std::string &parent_path, bool watch)
{
	std::vector<std::string> children;

	struct String_vector child_vec;

	int zoo_code = zoo_get_children(zoo_handle_, parent_path.c_str(), watch, &child_vec);
	CHECK_ZOOCODE_AND_THROW(zoo_code);

	children.reserve(child_vec.count);

	for (int i = 0; i < child_vec.count; ++i)
	{
		children.push_back(child_vec.data[i]);
	}

	// TODO: release child_vec
	return children;
}

}

