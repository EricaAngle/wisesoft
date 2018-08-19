#include "udpthreadsender.h"
#include <QUdpSocket>
#include <QDateTime>
#include <QByteArray>
#include <QDataStream>
#include <iostream>
#include <thread>

struct PrivateUdpThreadSenderData{
    std::shared_ptr<QUdpSocket> socket;
    quint16 port = 11001;
};

UdpThreadSender::UdpThreadSender():_d(new PrivateUdpThreadSenderData)
{

}

UdpThreadSender::~UdpThreadSender(){

}

void
UdpThreadSender::operator ()(){
    int sndcount= 100;
    _d->socket.reset(new QUdpSocket());
    while(sndcount>0 ){
       --sndcount;
        QByteArray bdata;
        QDataStream dstream(&bdata,QIODevice::WriteOnly);
        dstream<<QDateTime::currentDateTime();

        _d->socket->writeDatagram(bdata,QHostAddress::LocalHost,_d->port);
        std::cout<<"Sending Data : "<<sndcount<<std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}
