#include "GameServer.h"
#include "Data/DataSetting.h"

#include <iostream>

USING_NS_CC;

typedef ExperimentalSendData::Dat Dat;

GameServer::GameServer(const std::string &ip, u_short port) :
    TcpServer(ip, port)
{

}

GameServer::GameServer(const std::string &ip, const std::string &port) :
    TcpServer(ip, (u_short)Value(port).asInt())
{

}

void GameServer::read()
{
//    printf("recv buffer[%s]", buffer);
    dat = Dat(dat.toStr() + buffer);
    msgToSend = dat.toStr();
}
