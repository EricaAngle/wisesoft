#ifndef zk_client_h__
#define zk_client_h__

#include <zookeeper.h>
 
#include <string>
namespace zk
{

class ZkClient
{
public:
	static ZkClient& Instance();
	~ZkClient();

	void ConnectZK();

	void Update();

	void ServiceWatcher(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx) ;

	void InitWatcher(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx);

private:
	ZkClient();
	void Init();
private:
	zhandle_t *zhandle_;
	bool connected_;
	int timeout_;
	std::string zk_hosts_;	
	

};
}

//end of namespace zk

#endif