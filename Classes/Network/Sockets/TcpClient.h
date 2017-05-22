#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <string>
#include <future>

#include "TcpSocket.h"

class TcpClient : public TcpSocket
{

public:
    TcpClient(const std::string &ip, u_short port);
   ~TcpClient();

public:
    virtual void loop(const float delayMSec) override;

public:
    virtual void read() = 0;
    void loopReceiver();
    void loopSender();


    std::string getMsgToSend() const;
    void setMsgToSend(const std::string &value);
};

#endif // TCPCLIENT_H
