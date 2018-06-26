#ifndef myzoowatcher_h__
#define myzoowatcher_h__
#include "abstractwatcher.hpp"
namespace zk
{
class MyZooWatcher:public ZooWatcher
{
public:
	virtual ~MyZooWatcher() ;

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
#endif // myzoowatcher_h__
