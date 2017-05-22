#ifndef __LOCALPLAYER_H__
#define __LOCALPLAYER_H__


#include "cocos2d.h"
#include "Snake.h"

#include "Actions/NormalEat.h"
#include "Actions/Bonus.h"

#include "Layers/LevelLayer.h"
#include "Layers/GameNavigatorLayer.h"

#include "Data/SendData.h"

#include "Network/GameClient.h"
#include "Network/PlayerData.h"



NS_CC_BEGIN

typedef ExperimentalSendData::TypeData TypeData;


class LocalPlayer : public cocos2d::Snake
{

    Bonus *bonus;
    NormalEat *eat;
    GameNavigatorLayer *gnlayer;
    GameClient *gameClient;
    LevelLayer *levelLayer;
    Snake *listen;
    bool isRunningServer;
    std::pair<bool, std::string> statusShot;

    std::function<void (const Vec2 &in, const Vec2 &to)> callbackShotBomba;

public:
   ~LocalPlayer();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(LocalPlayer)

    Snake *getListen() const;

    void setListen(Snake *value);

    void setEat(NormalEat *value);

    void setLevelLayer(LevelLayer *layer);

    void setIsRunningServer(bool value);

    void setBonus(Bonus *value);

    void setGNLayer(GameNavigatorLayer *value);

    std::pair<bool, std::string> getStatusShot() const;
    void setStatusShot(const std::pair<bool, std::string> &value);

    void setCallbackShotBomba(const std::function<void (const Vec2 &in, const Vec2 &to)> &value);

    GameClient *getGameClient() const;

private:
    ExperimentalSendData *snd;
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::Vec2 currentPos;

private:
    std::function<void(ExperimentalSendData::Dat data)> getCallback();

private:
    void initGameClient();
    void acceptRecvData(Node *ptr, TypeData type, ExperimentalSendData::Dat dat,
                        const std::function<void (const std::string &source)> &callback);

protected:
    virtual void movingHead(float delta);

};

NS_CC_END

#endif // __LOCALPLAYER_H__
