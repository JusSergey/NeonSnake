#include "GameServer.h"
#include "Data/DataSetting.h"

#include <iostream>

USING_NS_CC;

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

    for (std::string &str : DataSetting::stringToVector(buffer, '\n')){

        auto pos = str.find(' ');

        std::string name = str.substr(0, pos);

        DataBasePlayers[name] = str.c_str() + pos + 1;
    }

    std::string result;
    for (auto i : DataBasePlayers)
        result += i.first + ' ' + i.second + '\n';

    msgToSend = result;
}
