#include "LocalPlayer.h"
#include "Data/DataSetting.h"
#include "Data/SendData.h"
#include <iostream>

typedef ExperimentalSendData::TypeData TypeData;

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

        ExperimentalSendData *snd = ExperimentalSendData::getInst();
        snd->dat.clear();



        if (listen) {
            snd->dat.set(isRunningServer ? TypeData::POS_PLAYER : TypeData::POS_OPPONENT,
                         listen->getPosition() * sclFactor);
        }
        if (isRunningServer && eat) {
            snd->dat.set(TypeData::POS_EAT, eat->getPosition() * sclFactor);
        }

        gameClient->setMsgToSend(/*message + "\n------\n" + */snd->toStr());

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

std::function<void (ExperimentalSendData::Dat data)> LocalPlayer::getCallback()
{
    float sclFactor = _director->getContentScaleFactor();
    return [sclFactor, this](ExperimentalSendData::Dat data) -> void {

        std::string str = data[isRunningServer ? TypeData::POS_OPPONENT : TypeData::POS_PLAYER];

        if (str.empty()) {
            log("LocalPlayer::getCallback(): empty data");
            return;
        }

        Vec2 tmp = ExperimentalSendData::toVec2(str);

        if (tmp.x > -1 && tmp.y > -1){
            currentPos = tmp /= sclFactor;
            head->runAction(MoveTo::create(1.1f / UpdateRecver, currentPos));
        }

//        if (!isRunningServer && eat){
//            data.eatPos /= sclFactor;
//            if (eat->getPosition() != data.eatPos)
//                eat->setPosition(data.eatPos);
//        }
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
