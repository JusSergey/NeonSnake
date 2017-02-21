#ifndef TCPSERVER_H
#define TCPSERVER_H

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
#include <list>
#include <map>
#include <mutex>
#include <future>
#include "TcpSocket.h"

class TcpServer : public TcpSocket
{
    int tmpFd;

protected:
    virtual void loop() override;

private:
    std::list<int> clients;
    std::map<int, int> statistic;

public:
    TcpServer(const std::string &ipServer, u_short port);
   ~TcpServer();

public:
    void loopListener();
    void loopReceiver();
    void loopSender();

protected:
    virtual void read() = 0;

};

#endif // TCPSERVER_H
