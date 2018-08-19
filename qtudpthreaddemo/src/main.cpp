//#include <QCoreApplication>
#include <QApplication>
//#include <thread>
//#include "udpthreadsender.h"
//#include "receivethread.h"
#include "mainwin.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    angle::MainWin win;
    win.show();

//    UdpThreadSender sender;
//    std::thread senderthread(std::ref<UdpThreadSender>(sender));

//    ReceiveThread rcvthrd;
//    rcvthrd.start();


   // senderthread.join();
    return a.exec();
}

