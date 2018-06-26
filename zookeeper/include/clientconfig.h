#ifndef clientconfig_h__
#define clientconfig_h__
#include <map>
#include <vector>
namespace zk {
	class ClientConfig{
	public:
		static ClientConfig& instance();
		~ClientConfig();

		const std::string& getHost() const{
			return _zkserver;
		}

		int getTimeout() const {
			return _timeout;
		}

		int getMaxTryTime() const {
			return _max_try_times;
		}

		const std::string& rootPath() const{
			return _rootpath;
		}


	private:
		ClientConfig(); 

		ClientConfig(const ClientConfig& other);

		ClientConfig& operator=(const ClientConfig& other);  //disable copy
		


	private:
		int _timeout;
		int _max_try_times;
		std::string _zkserver;
		std::string _rootpath;
		std::vector<std::string> _monitorsubpaths;

	};

}//end of namespace zk 
#endif // clientconfig_h__