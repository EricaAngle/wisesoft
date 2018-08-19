#include "receivethread.h"

#include "udpreceiver.h"

struct PrivateReceivThreadData{

};

ReceiveThread::ReceiveThread():_d(new PrivateReceivThreadData)
{

}


void
ReceiveThread::run(){
    UdpReceiver rcver;
    rcver.initialize();
    exec();
}

