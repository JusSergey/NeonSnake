#ifndef __LOCALPLAYER_H__
#define __LOCALPLAYER_H__

#include "cocos2d.h"
#include "Snake.h"

#include "Actions/NormalEat.h"

#include "Network/GameClient.h"
#include "Network/PlayerData.h"

NS_CC_BEGIN

class LocalPlayer : public cocos2d::Snake
{
    NormalEat *eat;
    GameClient *gameClient;
    Snake *listen;
    bool isRunningServer;

public:
   ~LocalPlayer();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(LocalPlayer);

    Snake *getListen() const;
    void setListen(Snake *value);

    void setEat(NormalEat *value);

    void setIsRunningServer(bool value);

private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::Vec2 currentPos;

private:
    std::function<void(ExperimentalSendData::Dat data)> getCallback();

private:
    void initGameClient();

protected:
    virtual void movingHead(float delta);

};

NS_CC_END

#endif // __LOCALPLAYER_H__
