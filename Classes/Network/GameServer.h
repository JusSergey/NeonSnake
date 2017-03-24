#ifndef __GAMESERVER_H__
#define __GAMESERVER_H__

#include "cocos2d.h"

#include "Data/SendData.h"

#include "Sockets/TcpServer.h"
#include "PlayerData.h"
#include <list>

class GameServer : public TcpServer
{
//    std::map<std::string, PlayerData> playersInfo;

    std::map<std::string, std::string> DataBasePlayers;

public:
    GameServer(const std::string &ip, u_short port);
    GameServer(const std::string &ip, const std::string &port);

protected:
    virtual void read();

private:
    ExperimentalSendData::Dat dat;

};

#endif // __GAMESERVER_H__
