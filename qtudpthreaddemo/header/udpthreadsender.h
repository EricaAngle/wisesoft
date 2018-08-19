#ifndef UDPTHREADSENDER_H
#define UDPTHREADSENDER_H
#include <memory>

struct PrivateUdpThreadSenderData;

class UdpThreadSender
{
public:
    UdpThreadSender();

    ~UdpThreadSender();

    void operator() ();

private:
    std::shared_ptr<PrivateUdpThreadSenderData> _d;
};

#endif // UDPTHREADSENDER_H
