#include "zooservermgr.h"
#include "myzoowatcher.h"
#include "ServerConfig.h"
#include "zookeeper.hpp"
#include "zookeeper_ext.hpp"
#include "zookeeper_error.hpp"

namespace zk
{
struct ZooServerJobPrivate
{
	zk::ZooKeeper zpublisher;
	zk::MyZooWatcher watcher;
	std::vector<std::string>  subnodelist;
	std::string  nodename;

};

ZooServerJob::ZooServerJob()
{
	_d =  new ZooServerJobPrivate();
}

ZooServerJob::~ZooServerJob()
{
	try
	{
		for (std::vector<std::string>::const_iterator citer = _d->subnodelist.begin();
			 citer != _d->subnodelist.end(); ++citer)
		{
			_d->zpublisher.DeleteIfExists(*citer);
		}

		_d->zpublisher.DeleteIfExists(_d->nodename);
	}
	catch (ZooException exp)
	{
		// catch exceptions
	}

	delete _d;
}

void ZooServerJob::ConnectZK()
{
	zk::ServerConfig &conf = zk::ServerConfig::instance();

	try
	{
		_d->zpublisher.connectZK(conf.getHost(), &(_d->watcher), conf.getTimeOut(), conf.maxTryTime());
	}
	catch (ZooException exp)
	{
		// log
	}
}

void ZooServerJob::createNode()
{
	zk::ServerConfig &conf = zk::ServerConfig::instance();

	try
	{
		_d->zpublisher.CreateIfNotExists(conf.rootNode(), std::string("version:1.0"), 0);
		std::string subnode = conf.rootNode() + "/simnode";
		std::string nodetype = _d->zpublisher.CreateIfNotExists(subnode, std::string("version:1.0"), 0);


		_d->zpublisher.CreateIfNotExists(subnode + "/" + conf.nodeName(), conf.ipv4_port(), ZOO_EPHEMERAL);
		_d->nodename = _d->zpublisher.CreateIfNotExists(conf.rootNode() + "/" + conf.nodeName(), "persistent_node_name", 0);
		std::map<std::string, std::string>::const_iterator iter = conf.secondNode().begin();

		for (iter; iter != conf.secondNode().end(); ++iter)
		{
			// create ephemeral node. which will be deleted after programming quit.
			std::string subnode = _d->nodename + iter->first;
			_d->subnodelist.push_back(subnode);
			_d->zpublisher.CreateIfNotExists(subnode, iter->second, ZOO_EPHEMERAL);
		}
	}
	catch (ZooException exp)
	{
		if (exp.code() == ZNODEEXISTS)
		{
			//create sub node.
		}
	}
}

bool ZooServerJob::updateNodeValue(const std::string &nodepath, const std::string &nodevalue)
{
	if (!_d->zpublisher.is_connected())
		return false;

	if (!_d->zpublisher.is_expired())
		return false;

	zk::NodeStat nodestat;

	if (_d->zpublisher.Exists(nodepath, true, &nodestat))
	{
		try
		{
			_d->zpublisher.Set(nodepath, nodevalue);
		}
		catch (ZooException exp)
		{
			return false;
		}
	}

	return true;
}


}