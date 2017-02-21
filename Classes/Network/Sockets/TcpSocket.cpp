#include "TcpSocket.h"
#include <fcntl.h>
#include <unistd.h>
#include "string.h"
#include <iostream>

// created by qt. Sergey Juice

TcpSocket::TcpSocket(const std::string &ip, u_short port) :
    fd(-1),
    ip(ip),
    port(port),
    timeListener(0),
    timeReceiver(0),
    timeSender(0),
    statusThread(true)
{
    bzero(&sock, sizeof(sock));

    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd < 0)
        err(fd, "socket()");

    sock.sin_family = AF_INET;
    sock.sin_port = htons(port);
    sock.sin_addr.s_addr = inet_addr(ip.c_str());

}

TcpSocket::~TcpSocket()
{
    statusThread = false;
    futureOfThread.wait();
    close(fd);
}

void TcpSocket::unlockFD(int _fd)
{
    int f;

    if ((f = fcntl(_fd, F_GETFL, 0x00000000000)) == -1 ||
             fcntl(_fd, F_SETFL, f | O_NONBLOCK) == -1)
    {
        err(1, "fcntl()");
    }
}

void TcpSocket::initThread()
{
    futureOfThread = std::async(std::launch::async, [this](){
        while (true) {
            mut.lock();
            if (!statusThread) break;
            loop();
            mut.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            timeListener++;
            timeReceiver++;
            timeSender++;
        }
    });
}
