#include "LocalPlayer.h"
#include "Data/DataSetting.h"
//#include "Constant.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool LocalPlayer::init()
{
    //////////////////////////////
    // 1. super init first
    if(!Snake::init())
        return false;

    isRunningServer = false;

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

    schedule([this](float){
        std::string message;
        if (listen) {
            message += GameClient::TypeData::Snake_t;
            message += UserData::playerName + ' ';
            message += StringUtils::toString((int)listen->getPositionX()) + ' ';
            message += StringUtils::toString((int)listen->getPositionY()) + '\n';
        }
        if (isRunningServer && eat) {
            message += GameClient::TypeData::Eat;
            message += ' ';
            message += StringUtils::toString((int)eat->getPositionX()) + ' ';
            message += StringUtils::toString((int)eat->getPositionY()) + '\n';
        }
        gameClient->setMsgToSend(message);
    }, updateServer, "sendmessagestoserver");

}

void LocalPlayer::movingHead(float delta)
{
//    head->setPosition(currentPos);
//    head->runAction(MoveTo::create(delta, currentPos));
}

Snake *LocalPlayer::getListen() const
{
    return listen;
}

void LocalPlayer::setListen(Snake *value)
{
    listen = value;
}

std::function<void (PlayerData data)> LocalPlayer::getCallback()
{
    return [this](PlayerData data) {

        if (data.opponentPos.x > -1 && data.opponentPos.y > -1){
            currentPos = data.opponentPos;
            head->runAction(MoveTo::create(1.1f / UpdateRecver, currentPos));
        }

        if (!isRunningServer && eat)
            if (eat->getPosition() != data.eatPos)
                eat->setPosition(data.eatPos);
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
}
