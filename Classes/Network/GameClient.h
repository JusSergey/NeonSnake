#ifndef __GAMECLIENT_H__
#define __GAMECLIENT_H__

#include "cocos2d.h"
#include "Sockets/TcpClient.h"
#include <functional>
#include "PlayerData.h"

class GameClient : public TcpClient
{
public:
    enum TypeData: char {
        Snake_t = 's',
        Eat = 'e'
    };
    GameClient(const std::string &ip, const std::string &port);

    void setCallbackRead(const std::function<void (PlayerData)> &value);

protected:
    virtual void read();
    std::function<void(PlayerData)> callbackRead;

};

#endif // __GAMECLIENT_H__
