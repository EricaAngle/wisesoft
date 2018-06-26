#include "zkregistry.h"
#include "serverconfig.h"
#include <functional>
#include <iostream>

#define SLOG_WARN(a ) std::cout<<"Warn: "<<a <<std::endl;
#define SLOG_INFO(a ) std::cout<<"Info: "<<a <<std::endl;

#define  PING_SERVER   "/pingserver"

namespace zk
{



static void WatcherService(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx)
{

}

static void WatcherInit(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx)
{
	std::cout << "ZooKeeper Connect Initialized! " << "type: " << type << " state: " << state << " path: " << path << std::endl;
	ZkRegistry::instance().InitWatcher(zh, type, state, path, watcherCtx);
}

const std::string &ZkRegistry::GetIp() const
{
	return ip_;
}
ZkRegistry &
ZkRegistry::instance()
{
	static ZkRegistry d;
	return d;
}

ZkRegistry::ZkRegistry():
	zhandle_(NULL),
	connected_(false),
	zk_hosts_("192.168.1.103:2181"),
	timeout_(1000),
	server_path(std::string("platform-simulation")),
	ip_(std::string("192.168.1.101"))
{

}

ZkRegistry::~ZkRegistry()
{
	zookeeper_close(zhandle_);

}

const int ZK_MAX_CONNECT_TIMES = 10;
/*
@brief connect to zookeeper
*/
void ZkRegistry::ConnectZK()
{
	std::cout << "connect to ZooKeeper";

	if (zhandle_)
	{
		zookeeper_close(zhandle_);
	}

	int count = 0;

	do
	{
		++count;

		zhandle_ = zookeeper_init(zk_hosts_.c_str(), WatcherInit, timeout_, NULL, NULL, 0);
	}
	while (!connected_ && count < ZK_MAX_CONNECT_TIMES);

	if (count >= ZK_MAX_CONNECT_TIMES)
	{
		SLOG_WARN("ZkRegistry::Init --> connect host " << zk_hosts_ << " over max times:" << count);
		return;
	}
}

//public server . create temporary node.
void ZkRegistry::PublishService()
{
	if (zhandle_ == NULL)
	{
		ConnectZK();
	}

	Stat stat;
	int ret = 0;

	std::string server_path = std::string(PING_SERVER) + "/" + ServerConfig::instance().get_index() + "/"
							  + GetIp() + ":" + ServerConfig::instance().get_port();
	//check exist
	ret = zoo_exists(zhandle_, server_path.c_str(), 0, &stat);



	if (ZNONODE == ret)
	{
		char res_path[128] = { 0 };
		int rc = zoo_create(zhandle_, server_path.c_str(), GetIp().c_str(), GetIp().size(),
							&ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL | ZOO_SEQUENCE,  res_path, 128);
		std::cout << std::string(res_path) << std::endl;

		if (rc)
		{
			SLOG_INFO("ZkRegistry::PublishService --> zoo_create() path=" << server_path << "," << zerror(rc));
		}
	}
}


void ZkRegistry::run()
{
	while (true)
	{
		SLOG_INFO("publish service");
		ConnectZK();
		PublishService();
		//sleep(10);
		Sleep(10 * 1000);
	}
}

void ZkRegistry::InitWatcher(zhandle_t *zh, int type, int state, const char *path, void *watcher_ctx)
{
	if (state == ZOO_CONNECTED_STATE)
	{
		connected_ = true;
		SLOG_INFO("InitWatcher() ZOO_CONNECTED_STATE");
	}
	else if (state == ZOO_AUTH_FAILED_STATE)
	{
		SLOG_INFO("InitWatcher() ZOO_AUTH_FAILED_STATE");
	}
	else if (state == ZOO_EXPIRED_SESSION_STATE)
	{
		SLOG_INFO("InitWatcher() ZOO_EXPIRED_SESSION_STATE");
	}
	else if (state == ZOO_CONNECTING_STATE)
	{
		SLOG_INFO("InitWatcher() ZOO_CONNECTING_STATE");
	}
	else if (state == ZOO_ASSOCIATING_STATE)
	{
		SLOG_INFO("InitWatcher() ZOO_ASSOCIATING_STATE");
	}
}

}