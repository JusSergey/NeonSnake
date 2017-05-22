#include "GameClient.h"
#include "Data/DataSetting.h"
#include <iostream>
#include <fstream>
using namespace std;

USING_NS_CC;

#define npos std::string::npos

GameClient::GameClient(const std::string &ip, const std::string &port) :
    TcpClient(ip, (u_short)Value(port).asInt()),
    callbackRead([](ExperimentalSendData::Dat){})
{
}

GameClient::~GameClient()
{

}

void GameClient::read()
{
    if (statusThread)
        callbackRead(ExperimentalSendData::Dat(buffer));

}

void GameClient::setCallbackRead(const std::function<void (ExperimentalSendData::Dat)> &value)
{
    callbackRead = value;
}
