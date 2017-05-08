#include "GameServer.h"
#include "Data/DataSetting.h"

#include <iostream>
#include <fstream>
using namespace std;

USING_NS_CC;

typedef ExperimentalSendData::Dat Dat;

GameServer::GameServer(const std::string &ip, u_short port) :
    TcpServer(ip, port),
    logServer(ServerLogFilePath, ios::app)
{

}

GameServer::GameServer(const std::string &ip, const std::string &port) :
    TcpServer(ip, (u_short)Value(port).asInt()),
    logServer(ServerLogFilePath, ios::app)
{

}

GameServer::~GameServer()
{
    logServer.flush();
    logServer.close();
}

void GameServer::read()
{
//    printf("recv buffer[%s]", buffer);
    dat = Dat(dat.toStr() + buffer);
    msgToSend = dat.toStr();

    logServer << msgToSend << "\n----------\n";
}
