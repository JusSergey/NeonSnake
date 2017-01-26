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
    printInfo();

    initThread();
}

TcpServer::~TcpServer()
{
    for (auto i : clients)
        close(i);
}

void TcpServer::loop()
{
    if (timeListener > toMsec(UpdateListener)) {
        loopListener();
        timeListener = 0;
    }
    if (timeReceiver > toMsec(UpdateRecver)) {
        loopReceiver();
        timeReceiver = 0;
    }
    if (timeSender > toMsec(UpdateSender)) {
        loopSender();
        timeSender = 0;
    }
}

void TcpServer::loopListener()
{
    tmpFd = accept(fd, NULL, NULL);

    if (tmpFd > 0) {
        unlockFD(tmpFd);
        clients.push_front(tmpFd);
        statistic[tmpFd] = 0;

        printInfo();

        std::cout.flush();
    }

}

void TcpServer::loopReceiver()
{
    for (int i : clients)
        if (recv(i, buffer, sizeof(buffer), MSG_WAITALL) > 0)
            read();
}

void TcpServer::loopSender()
{
    for (auto i : clients) {

        ssize_t bytes = send(i, msgToSend.c_str(), msgToSend.size()+1, MSG_NOSIGNAL);

        if (bytes == -1) {

            int &r = statistic[i];
            r++;

            if (r > 10){
                clients.remove(i);
                printInfo();
                break;
            }
        }

    }

}

void TcpServer::printInfo()
{
    system("clear");

    std::cout << "Count users: " << clients.size() << '\n';

    for (auto i : clients)
        std::cout << "id: " << i << '\n';
}

