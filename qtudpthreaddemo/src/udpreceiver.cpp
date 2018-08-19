#include "udpreceiver.h"
#include <QUdpSocket>
#include <QDataStream>
#include <QDateTime>
#include <QByteArray>
#include <QDebug>
#include <iostream>

struct PrivateUdpReceiverData{
QUdpSocket * rcvskt;
};

UdpReceiver::UdpReceiver(QObject *parent) : QObject(parent),_d(new PrivateUdpReceiverData)
{

}


UdpReceiver::~UdpReceiver(){
    qDebug()<<"Release";
}


void
UdpReceiver::initialize(){
    _d->rcvskt = new QUdpSocket(this);
    _d->rcvskt->bind(QHostAddress::LocalHost,11001,QAbstractSocket::ShareAddress);
    connect(_d->rcvskt, SIGNAL(readyRead()),this, SLOT(receiveData()));
}


void
UdpReceiver::receiveData(){
    QByteArray rcvdata;
    while(_d->rcvskt->hasPendingDatagrams()){
        rcvdata.resize(int(_d->rcvskt->pendingDatagramSize()));
        _d->rcvskt->readDatagram(rcvdata.data(),_d->rcvskt->pendingDatagramSize());
        //QByteArray rcvdata =_d->rcvskt->readAll();
        //std::cout<<rcvdata.size()<<std::endl;
        QDataStream dread(&rcvdata,QIODevice::ReadOnly);
        QDateTime dt;
        dread>>dt;
        std::cout<<(dt.toString(Qt::ISODate).toStdString())<<std::endl;
    }
}
