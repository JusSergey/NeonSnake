#include "TcpServer.h"
#include <iostream>
#include <thread>
#include <future>
#include <err.h>
#include <fcntl.h>

TcpServer::TcpServer(const std::string &ipServer, u_short port) :
    TcpSocket(ipServer, port),
    tmpFd(-1)
{
    if(bind(fd, (struct sockaddr *)&sock, sizeof(sock)) < 0)
        err(0, "bind()");

    unlockFD(fd);

    listen(fd, 5);

//    initThread();
}

TcpServer::~TcpServer()
{
    for (auto i : clients)
        close(i);
}

void TcpServer::loop(const float delayMSec)
{
    TcpSocket::loop(delayMSec);
    if (timeListener > toMsec(UpdateListener)) {
        loopListener();
        timeListener = 0;
    }
    if (timeReceiver > toMsec(UpdateRecver)) {
        loopReceiver();
        timeReceiver = 0;
//        delayMsecSend= 0;
    }
    if (timeSender > toMsec(UpdateSender)) {
        loopSender();
        timeSender = 0;
//        delayMsecRecv = 0;
    }
}

void TcpServer::loopListener()
{
    tmpFd = accept(fd, NULL, NULL);

    if (tmpFd > 0) {
        unlockFD(tmpFd);
        clients.push_front(tmpFd);
        statistic[tmpFd] = 0;

        std::cout.flush();
    }

}

void TcpServer::loopReceiver()
{
    for (int i : clients)
        if (recv(i, buffer, sizeof(buffer), MSG_NOSIGNAL) > 0)
            read();
}

void TcpServer::loopSender()
{
    for (auto i : clients) {

        ssize_t bytes = send(i, msgToSend.c_str(), msgToSend.size()+1, MSG_NOSIGNAL);

        if (bytes <= 0) {

            int &r = statistic[i];
            r++;

            (std::cout << "no signal of socket").flush();

            if (r > 10){
//                (std::cout << "remove of clients.").flush();
                clients.remove(i);
                break;
            }
        }

    }

}

