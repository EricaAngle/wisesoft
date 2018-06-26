#ifndef zookeeper_h__
#define zookeeper_h__

#include <zookeeper.h>
#include "abstractwatcher.hpp"
#include <string>
#include <sstream>
#include <vector>

namespace zk
{

class ZooKeeperConf
{
public:
	std::string host;
	int port;
	unsigned int timeout ;
	std::string username;
	std::string password;
	std::string digest;
	struct ACL_vector aclv;

	std::string connectStr() const
	{
		std::stringbuf strbuf;
		std::ostream os(&strbuf);
		os << host << ":" << port;
		return strbuf.str();
	}
	ZooKeeperConf():host(std::string("192.168.1.103")),port(2181), timeout( 4000),
		digest(std::string("digest"))
	{
		char scheme[] = "digest";
		char usr_pa[] = "test:V28q/NynI4JI3Rk54h0r8O5kMug=";

		aclv.count = 1;
		struct ACL acl;
		acl.perms = ZOO_PERM_ALL;
		acl.id.scheme = scheme;
		acl.id.id = usr_pa;
		aclv.data = &acl;
	}
};

typedef Stat NodeStat;

class ZooKeeper
{
public:
	explicit ZooKeeper();

	ZooKeeper(const std::string &server_hosts,
			  ZooWatcher *global_watcher = NULL,
			  int timeout_ms = 5 * 1000);

	ZooKeeper(const ZooKeeperConf &conf);

	~ZooKeeper();

	bool connectZK(const std::string &server_hosts,
		ZooWatcher *global_watcher,
		int timeout_ms, int max_try_time);

	bool is_connected();
	bool is_expired();

	bool Exists(const std::string &path, bool watch = false, NodeStat * = NULL);

	NodeStat Stat(const std::string &path);

	std::string Create(const std::string &path,
					   const std::string &value = std::string(),
					   int flag = 0);
	std::string Create(const std::string& path, const std::string& value, struct ACL_vector&, int flag = 0);

	int add_Auth(const std::string& scheme/*digest*/, const std::string& username, const std::string& password);

	std::string CreateIfNotExists(const std::string &path,
								  const std::string &value = std::string(),
								  int flag = 0);

	void Delete(const std::string &path);

	void DeleteIfExists(const std::string &path);

	void Set(const std::string &path, const std::string &value);

	std::string Get(const std::string &path, bool watch = false);

	std::vector<std::string> GetChildren(const std::string &parent_path, bool watch = false);

private:
	// disable copy
	ZooKeeper(const ZooKeeper &) ;
	ZooKeeper &operator=(const ZooKeeper &) ;


	void WatchHandler(int type, int state, const char *path);

	static void GlobalWatchFunc(zhandle_t *, int type, int state,
								const char *path, void *ctx);

private:
	zhandle_t    *zoo_handle_ ;

	clientid_t   *_client_conect_id ;

	ZooWatcher   *global_watcher_ ;

};

}

#endif // zookeeper_h__
