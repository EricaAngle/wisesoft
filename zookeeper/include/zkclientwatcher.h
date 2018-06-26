#ifndef zkclientwatcher_h__
#define zkclientwatcher_h__
#include "abstractwatcher.hpp"
namespace zk
{

class ZKClientWatcher: public ZooWatcher
{
public:
	virtual ~ZKClientWatcher();

	virtual void OnConnected() override;
	virtual void OnConnecting() override;
	virtual void OnSessionExpired() override;

	virtual void OnCreated(const char *path) override;
	virtual void OnDeleted(const char *path) override;
	virtual void OnChanged(const char *path) override;
	virtual void OnChildChanged(const char *path) override;
	virtual void OnNotWatching(const char *path) override;
};


}
#endif // zkclientwatcher_h__