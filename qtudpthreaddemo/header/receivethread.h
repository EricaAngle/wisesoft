#ifndef RECEIVETHREAD_H
#define RECEIVETHREAD_H

#include <QThread>
#include <memory>

struct PrivateReceivThreadData;

class ReceiveThread :public QThread
{
    Q_OBJECT
public:
    ReceiveThread();

protected:
    void run() override;

private:
    std::shared_ptr<PrivateReceivThreadData> _d;
};

#endif // RECEIVETHREAD_H
