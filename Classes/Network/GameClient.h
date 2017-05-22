#ifndef __GAMECLIENT_H__
#define __GAMECLIENT_H__

#include "cocos2d.h"
#include "Sockets/TcpClient.h"
#include <functional>
#include "PlayerData.h"
#include "Data/SendData.h"

class GameClient : public TcpClient
{
public:
    enum TypeData: char {
        Snake = 's',
        Eat = 'e'
    };
    GameClient(const std::string &ip, const std::string &port);
   ~GameClient();

    void setCallbackRead(const std::function<void (ExperimentalSendData::Dat)> &value);

protected:
    virtual void read();
    std::function<void(ExperimentalSendData::Dat)> callbackRead;

};

#endif // __GAMECLIENT_H__
