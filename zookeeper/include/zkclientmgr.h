#ifndef zkclientmgr_h__
#define zkclientmgr_h__
#include <string>
#include <vector>

namespace zk
{

struct ZKClientMgrPrivate;

class ZKClientMgr
{
public:
	ZKClientMgr();

	~ZKClientMgr();

	void connectZK();
	
	void update();

private:
	void walkThroughChildren(const std::string &path, std::vector<std::string> &children);

	ZKClientMgrPrivate *_d;
};

}
#endif // zkclientmgr_h__