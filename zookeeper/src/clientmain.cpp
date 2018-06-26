
#include "zkclientmgr.h"
#include "zkclient.h"

int main(int argc , char* argv[]){
	zk::ZKClientMgr zkc ;
	zkc.connectZK();
	int max_time = 50;
	int i = 0; 
	do{
		zkc.update();
		Sleep(2 * 1000);
		++i;
	} while (i < max_time);
	

	return 0;
    
}