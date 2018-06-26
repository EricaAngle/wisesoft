#ifndef zooservermgr_h__
#define zooservermgr_h__ 
#include <string>

namespace zk
{
struct ZooServerJobPrivate;
class ZooServerJob
{
public:
	explicit ZooServerJob();
	~ZooServerJob();
	void ConnectZK();
	void createNode();
	bool updateNodeValue(const std::string& nodepath, const std::string& nodevalue);
private:
	ZooServerJobPrivate* _d;
};
}
#endif // zooservermgr_h__
