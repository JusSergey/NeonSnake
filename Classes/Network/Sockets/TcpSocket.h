#ifndef __TCPSOCKET_H__
#define __TCPSOCKET_H__

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <err.h>

#include <future>
#include <mutex>

#define UpdateListener 10
#define UpdateSender 9
#define UpdateRecver 9
#define toMsec(count) (1.f / (count) * 1000)

class TcpSocket {

    std::future<void> futureOfThread;
    int delayMsecSend,
        delayMsecRecv;

protected:
    std::mutex mut;

public:
    TcpSocket(const std::string &ip, u_short port);
   ~TcpSocket();

    inline bool getStatus() const { return statusThread; }

protected:
    int timeListener,
        timeSender  ,
        timeReceiver;

protected: /* user func */
    void unlockFD(int _fd);
//    void initThread();

public:
    virtual void loop(const float delayMSec);

protected: /* data info of thread */
    volatile bool statusThread;


protected: /* data info to init */
    std::string ip;
    u_short port;


protected: /* data read/write buffers */
    std::string msgToSend;
    char buffer[4096];


protected: /* data to init socket */
    sockaddr_in sock;
    int fd;

};

#endif // __TCPSOCKET_H__
