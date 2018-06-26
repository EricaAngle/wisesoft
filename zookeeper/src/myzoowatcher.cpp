#include "myzoowatcher.h"
#include <iostream>
namespace zk
{

void MyZooWatcher::OnConnected()
{
	std::cout << __FUNCDNAME__ <<":" <<__LINE__<< std::endl;
}
void MyZooWatcher::OnConnecting()
{
	std::cout << __FUNCDNAME__ << std::endl;
}
void MyZooWatcher::OnSessionExpired()
{
	std::cout << __FUNCDNAME__ << std::endl;
}

void MyZooWatcher::OnCreated(const char *path)
{
	std::cout << __FUNCDNAME__ << std::endl;
}
void MyZooWatcher::OnDeleted(const char *path)
{
	std::cout << __FUNCDNAME__ <<" Path: "<<path<< std::endl;

}
void MyZooWatcher::OnChanged(const char *path)
{
	std::cout << __FUNCDNAME__ << std::endl;
}
void MyZooWatcher::OnChildChanged(const char *path)
{
	std::cout << __FUNCDNAME__ << std::endl;
}
void MyZooWatcher::OnNotWatching(const char *path)
{
	std::cout << __FUNCDNAME__ << std::endl;
}


MyZooWatcher::~MyZooWatcher()
{
	std::cout<<"free watcher";

}

}