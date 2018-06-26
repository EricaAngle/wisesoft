#include "zkclient.h"
#include <stdio.h>
#include <iostream>
using namespace std;

#define SLOG_WARN(a ) std::cout<<"Warn: "<<a <<std::endl;
#define SLOG_INFO(a ) std::cout<<"Info: "<<a <<std::endl;

namespace zk
{
const int  ONE_SECONDS = 1000;// ms
const int ZK_MAX_CONNECT_TIMES = 10;
static const std::string PING_SERVER = "/flybird";

ZkClient &ZkClient::Instance()
{
	static ZkClient d;
	return d;
}


ZkClient::ZkClient() : zhandle_(NULL),
connected_(false), timeout_(2000),zk_hosts_(std::string("10.2.2.106:2181,10.2.2.106:2182"))
{

}

void WatcherService(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx)
{
	ZkClient::Instance().InitWatcher(zh, type, state, path, watcherCtx);
}

void WatcherInit(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx)
{
	ZkClient::Instance().InitWatcher(zh, type, state, path, watcherCtx);
}


ZkClient::~ZkClient()
{

}
void
ZkClient::Init()
{

}

void
ZkClient::InitWatcher(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx)
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

//连接zk server
void ZkClient::ConnectZK()
{
	cout << "ZkClient::ConnectZK" << endl;

	if (zhandle_)
	{
		zookeeper_close(zhandle_);
	}

	zhandle_ = NULL;
	connected_ = false;

	int count = 0;

	do
	{
		++count;
		zhandle_ = zookeeper_init(zk_hosts_.c_str(), WatcherInit, timeout_, NULL, NULL, 0);

		Sleep(5 * ONE_SECONDS);
		//sleep(5 * ONE_SECONDS);
	}
	while (!connected_ && count < ZK_MAX_CONNECT_TIMES);

	if (count >= ZK_MAX_CONNECT_TIMES)
	{
		cout << "ZkClient::Init --> connecting zookeeper host: " << zk_hosts_ << " over times: " << count << endl;
	}
}
//更新服务列表，冷备和热备
void ZkClient::Update()
{
	cout << "ZkClient::Update" << endl;

	if (zhandle_ == NULL || connected_ == false)
	{
		Init();
	}

	//获得服务份数
	struct String_vector str_vec;
	std::string path("/flybird");
	std::string app_path("/app");
	char buffer[1024] = { 0 };
	int buffersize = sizeof(buffer);
	int ret = zoo_get(zhandle_, path.c_str(), true, buffer, &buffersize, NULL);
	if (ret == ZOK) {
		std::cout << path<<": "<< buffer << std::endl;
	}
	
	memset(buffer,0,buffersize);

	ret = zoo_get(zhandle_, app_path.c_str(),true, buffer, &buffersize, NULL);
	if( ret == ZOK) {
		std::cout<< app_path << ": "<< buffer << std::endl;
	}

	  ret = zoo_wget_children(zhandle_, path.c_str(), WatcherService, NULL, &str_vec);

	if (ret)
	{
		cout << "Update --> read path:" << PING_SERVER << " wrong, " << zerror(ret) << endl;
		return;
	}
	else {
		cout << "Update --> read path:" << path << " sucess, " << zerror(ret) << endl;
	}

	//获得各份服务ip:port
	for (int i = 0; i < str_vec.count; ++i)
	{
		struct String_vector node_vec;
		string path = PING_SERVER + "/" + str_vec.data[i];
		int ret = zoo_wget_children(zhandle_, path.c_str(), WatcherService, NULL, &node_vec);
		cout << "Update --> path:" << path << ", ret:" << ret << ", node's size:" << node_vec.count << endl;

		if (ret || node_vec.count != 1)
		{
			continue;
		}
	}
}
//monitor the change of service.
void ZkClient::ServiceWatcher(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx)
{
	cout << "type:" << type << endl;
	cout << "state:" << state << endl;
	cout << "path:" << path << endl;
	//  cout << "watcherCtx:" << (char*)watcherCtx << endl;
	cout << "ZOO_CHILD_EVENT:" << ZOO_CHILD_EVENT << endl;

	if (ZOO_CHILD_EVENT == type)
	{
		cout << "ServiceWatcher ZOO_CHILD_EVENT" << endl;
		ZkClient::Instance().Update();//refresh service table.
	}
}

}//end of namespace zk