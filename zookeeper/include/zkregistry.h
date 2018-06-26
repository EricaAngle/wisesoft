#ifndef zk_registry_h__
#define zk_registry_h__
#include <zookeeper.h>
#include <string>

namespace zk {
    class ZkRegistry{
      public:       

        ~ZkRegistry();
        
        void ConnectZK();

		static ZkRegistry& instance();
        
        void PublishService();
        
        void run();
        
        void InitWatcher(zhandle_t *zh, int type, int state, const char *path, void *watcher_ctx) ;   

		const std::string& GetIp() const; 
	private:
		ZkRegistry();
	private:
		zhandle_t* zhandle_;
		bool   connected_ ;
		std::string zk_hosts_;
		int timeout_;
		std::string server_path;
		std::string ip_;
    
	};
    
}

#endif //