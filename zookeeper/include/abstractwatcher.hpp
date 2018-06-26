#ifndef abstractwatcher_h__
#define abstractwatcher_h__



namespace zk
{

class ZooWatcher
{
public:
	virtual ~ZooWatcher() {}

	virtual void OnConnected() = 0;
	virtual void OnConnecting() = 0;
	virtual void OnSessionExpired() = 0;

	virtual void OnCreated(const char *path) = 0;
	virtual void OnDeleted(const char *path) = 0;
	virtual void OnChanged(const char *path) = 0;
	virtual void OnChildChanged(const char *path) = 0;
	virtual void OnNotWatching(const char *path) = 0;
};

}

#endif // abstractwatcher_h__