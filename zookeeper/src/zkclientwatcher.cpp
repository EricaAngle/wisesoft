#include "zkclientwatcher.h"
#include <iostream>
namespace zk
{
ZKClientWatcher::~ZKClientWatcher()
{

}

void ZKClientWatcher::OnConnected()
{
	std::cout<<"Connected"<< std::endl;
}

void ZKClientWatcher::OnConnecting()
{
	std::cout<<"Connecting State"<<std::endl;
}

void ZKClientWatcher::OnSessionExpired()
{
	std::cout<<"Sesssion Expired"<< std::endl;
}

void ZKClientWatcher::OnCreated(const char *path)
{
	std::cout<<"Create Path: "<< path<< std::endl;
}

void ZKClientWatcher::OnDeleted(const char *path)
{
	std::cout<<"Delete Path: "<< path<< std::endl;
}

void ZKClientWatcher::OnChanged(const char *path)
{
	std::cout<<"value change at path: "<< path<<std::endl;
}
void ZKClientWatcher::OnChildChanged(const char *path)
{
	std::cout<<"child path change at :"<< path<< std::endl;
}
void ZKClientWatcher::OnNotWatching(const char *path)
{
	std::cout<<"Not wathcher any more at :"<< path<< std::endl;
}

}