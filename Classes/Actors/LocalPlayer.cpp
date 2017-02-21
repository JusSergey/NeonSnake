#include "LocalPlayer.h"
#include "Data/DataSetting.h"
#include <iostream>

USING_NS_CC;

// on "init" you need to initialize your instance
bool LocalPlayer::init()
{
    //////////////////////////////
    // 1. super init first
    if(!Snake::init())
        return false;

    isRunningServer = false;

    gameClient = nullptr;
    listen = nullptr;
    eat = nullptr;

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    initGameClient();
    log("return if initGameClient");

    return true;
}

void LocalPlayer::initGameClient()
{

    gameClient = new GameClient(UserData::NetworkIp, UserData::NetworkPort);

    gameClient->setCallbackRead(getCallback());

    log("prev my rename");
    setName(NameBotOrOpponent);

    static const char *NameSch = "SendInfoToServer";

    static const float sclFactor = _director->getContentScaleFactor();

    schedule([=, this, NameSch](float){

        if (gameClient && !gameClient->getStatus()) {
            unschedule(NameSch);
            return;
        }

        static auto toValigPos = [] (const Vec2 &vec) -> Vec2 {

        };

        std::string message;

        if (listen) {
            message += GameClient::TypeData::Snake_t;
            message += UserData::playerName + ' ';
            message += StringUtils::toString( (int) (listen->getPositionX()*sclFactor) ) + ' ';
            message += StringUtils::toString( (int) (listen->getPositionY()*sclFactor) ) + '\n';
        }
        if (isRunningServer && eat) {
            message += GameClient::TypeData::Eat;
            message += ' ';
            message += StringUtils::toString( (int) (eat->getPositionX()*sclFactor) ) + ' ';
            message += StringUtils::toString( (int) (eat->getPositionY()*sclFactor) ) + '\n';
        }
        gameClient->setMsgToSend(message);

    }, updateServer, NameSch);

}

void LocalPlayer::movingHead(float delta) { }

Snake *LocalPlayer::getListen() const
{
    return listen;
}

void LocalPlayer::setListen(Snake *value)
{
    listen = value;
}

Vec2& operator /= (Vec2 &vec, float scl) {
    vec = vec / scl;
    return vec;
}

std::function<void (PlayerData data)> LocalPlayer::getCallback()
{
    float sclFactor = _director->getContentScaleFactor();
    return [sclFactor, this](PlayerData data) {

        if (data.opponentPos.x > -1 && data.opponentPos.y > -1){
            currentPos = data.opponentPos /= sclFactor;
            log("recv:[%f, %f]", currentPos.x, currentPos.y);
            head->runAction(MoveTo::create(1.1f / UpdateRecver, currentPos));
        }

        if (!isRunningServer && eat){
            data.eatPos /= sclFactor;
            if (eat->getPosition() != data.eatPos)
                eat->setPosition(data.eatPos);
        }
    };
}

void LocalPlayer::setEat(NormalEat *value)
{
    eat = value;
}

void LocalPlayer::setIsRunningServer(bool value)
{
    isRunningServer = value;
}

LocalPlayer::~LocalPlayer()
{
    delete gameClient;
    gameClient = nullptr;
}
