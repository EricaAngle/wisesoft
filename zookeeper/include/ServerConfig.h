#ifndef pingconfig_h__
#define pingconfig_h__
#include <string>
#include <map>
namespace zk
{

class ServerConfig
{
public:

	static ServerConfig &instance();
	const std::string &get_port() const ;

	const std::string &getHost() const
	{
		return _host;
	}

	int getTimeOut() const
	{
		return _timeout;
	}

	const int maxTryTime() const
	{
		return _max_connect_time;
	}

	const std::string &rootNode() const
	{
		return _rootpath;
	}

	const std::map<std::string, std::string> &secondNode() const
	{
		return _secondnodemap;
	}

	const std::string& get_index() const {
		return _index;
	}

	const std::string& nodeName() const{
		return _nodename;
	}
	const std::string& ipv4_port() const {
		return _ipv4_port;
	}

private:
	ServerConfig();

private: 
	int _timeout; //milliseconds
	int _max_connect_time ;
	int _node_id; // 
	std::string _nodename;
	std::string _index;
	std::string _port;
	std::string _host;	 
	std::string _rootpath ;
	std::string _ipv4_port;
	std::map<std::string, std::string> _secondnodemap;
};
}//end of namespace 
#endif // pingconfig_h__