#include "zkclientmgr.h"
#include "clientconfig.h"
#include "zookeeper.hpp"
#include "zookeeper_ext.hpp"
#include "zookeeper_error.hpp"
#include "zkclientwatcher.h"
#include <iostream>
#include <vector>

namespace zk
{
struct ZKClientMgrPrivate
{
	zk::ZooKeeper zbooker;
	zk::ZKClientWatcher watcher;
};

ZKClientMgr::ZKClientMgr(): _d(new ZKClientMgrPrivate)
{


}

ZKClientMgr::~ZKClientMgr()
{
	delete _d;
}

void ZKClientMgr::connectZK()
{
	const ClientConfig &conf = ClientConfig::instance();
	_d->zbooker.connectZK(conf.getHost(), &(_d->watcher), conf.getTimeout(), conf.getMaxTryTime());

}

void ZKClientMgr::update()
{
	const ClientConfig &conf = ClientConfig::instance();
	std::string value = _d->zbooker.Get(conf.rootPath(), true);
	std::cout << conf.rootPath() << ":=>" << value << std::endl;

	std::vector<std::string> children = _d->zbooker.GetChildren(conf.rootPath(), true);
	walkThroughChildren(conf.rootPath(), children);
}

void ZKClientMgr::walkThroughChildren(const std::string &path, std::vector<std::string> &children)
{
	if (children.size() <= 0) return;

	for (std::vector<std::string>::const_iterator citer = children.begin(); citer != children.end(); ++citer)
	{
		std::string subpath = path + "/" +  *citer;
		std::string val = _d->zbooker.Get(subpath, true);
		std::cout << "children:=>" << subpath << "  value: " << val << std::endl;
		std::vector<std::string> vecstr = _d->zbooker.GetChildren(subpath);
		walkThroughChildren(subpath, vecstr);
	}

}
}//end of namespace zk

