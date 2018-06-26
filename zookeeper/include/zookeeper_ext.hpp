#pragma once
#include <string>

namespace zk
{

class ZooKeeper;

std::string RecursiveCreate(ZooKeeper &zk,
							const std::string &path,
							const std::string &value = std::string(),
							int flag = 0);

}


