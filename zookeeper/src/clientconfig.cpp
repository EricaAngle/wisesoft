#include "clientconfig.h"

namespace zk {

	ClientConfig& ClientConfig::instance(){
		static ClientConfig d;
		return d;
	}

	ClientConfig::~ClientConfig(){

	}

	ClientConfig::ClientConfig():_timeout(5000),_max_try_times(5),_zkserver("10.2.2.106:2181"),
		_rootpath("/flybird")
	{
		_monitorsubpaths.push_back("/port");
		_monitorsubpaths.push_back("/topics");
		_monitorsubpaths.push_back("/name");
		_monitorsubpaths.push_back("/ipv4");
		_monitorsubpaths.push_back("/nodeid");
	}

	
}