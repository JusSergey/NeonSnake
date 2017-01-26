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

#define UpdateListener 5
#define UpdateSender 18
#define UpdateRecver 18
#define toMsec(count) (1.f / (count) * 1000)

class TcpSocket {

    std::future<void> futureOfThread;
    std::mutex mut;

public:
    TcpSocket(const std::string &ip, u_short port);
   ~TcpSocket();

protected:
    int timeListener,
        timeSender  ,
        timeReceiver;

protected: /* user func */
    void unlockFD(int _fd);
    void initThread();
    virtual void loop() = 0;

protected: /* data info of thread */
    bool statusThread;


protected: /* data info to init */
    std::string ip;
    u_short port;


protected: /* data read/write buffers */
    std::string msgToSend;
    char buffer[1024];


protected: /* data to init socket */
    sockaddr_in sock;
    int fd;

};

#endif // __TCPSOCKET_H__
