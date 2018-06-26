#include "serverconfig.h" 
#include <sstream>
namespace zk
{

#define  k_max_server_id ( 50000)

ServerConfig::ServerConfig():
_timeout( 15000),
_node_id(1001),
_max_connect_time (5), 
	_port(std::string("8001")),
	_host(std::string("10.2.2.106:2181,10.2.2.106:2182")),
	_rootpath(  std::string("/flybird")),
	_ipv4_port(std::string("10.2.2.101:8000"))
{

	std::stringstream strout;
	strout<<"sim_platform_"<<_node_id;
	
	_nodename=strout.str();
	_index="digest";
	char buffer_node_id[100] = {0};
	if (_node_id> k_max_server_id)
		_node_id%= k_max_server_id;
	itoa( _node_id, buffer_node_id,10);
	_secondnodemap.insert(std::pair<std::string, std::string>("/nodeid", std::string(buffer_node_id)));
	_secondnodemap.insert(std::pair<std::string, std::string>("/port", "8000"));
	_secondnodemap.insert(std::pair<std::string, std::string>("/topics", "a,b,c"));
	_secondnodemap.insert(std::pair<std::string, std::string>("/name", "control-master")); 
	_secondnodemap.insert(std::pair<std::string, std::string>("/status","active"));
	
}

ServerConfig &ServerConfig::instance()
{
	static ServerConfig d;
	return d;
}


 

const std::string &ServerConfig::get_port() const
{
	return _port;
}

}