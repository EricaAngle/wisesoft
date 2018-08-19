#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include <QObject>
#include <memory>

struct PrivateUdpReceiverData;

class UdpReceiver : public QObject
{
    Q_OBJECT
public:
    explicit UdpReceiver(QObject *parent = 0);

    ~UdpReceiver();

    void initialize();



signals:

public slots:
    void receiveData();

private:
    std::shared_ptr<PrivateUdpReceiverData> _d;
};

#endif // UDPRECEIVER_H
