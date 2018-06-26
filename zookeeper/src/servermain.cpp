#include "zooservermgr.h"


#include <iostream>
int main(int ac, char *argv[])
{
	zk::ZooServerJob job;
	job.ConnectZK();
	job.createNode();

	system("pause");
	return 0;
}