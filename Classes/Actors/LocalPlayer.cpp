#include "LocalPlayer.h"
#include "Data/DataSetting.h"
#include "Scenes/GameView.h"
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

    callbackShotBomba = [] (const Vec2 &, const Vec2 &) {};

    isRunningServer = false;

    gnlayer = nullptr;
    levelLayer = nullptr;
    gameClient = nullptr;
    listen = nullptr;
    bonus = nullptr;
    eat = nullptr;

    statusShot = std::make_pair(false, std::string(""));

    snd = ExperimentalSendData::getInst();

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

    setName(NameBotOrOpponent);

    static const char *NameSch = "SendInfoToServer";

    static const float sclFactor = _director->getContentScaleFactor();

    schedule([=, this, NameSch](float){

        if (gameClient && !gameClient->getStatus()) {
            unschedule(NameSch);
            return;
        }

        snd->dat.clear();

        if (listen) {
            snd->dat.set(isRunningServer ? TypeData::POS_PLAYER : TypeData::POS_OPPONENT,
                         listen->getPosition() * sclFactor);
        }

        if (isRunningServer) {

            if (listen && listen->getOpponent()) {
                if (gnlayer) {
                    snd->dat.set(TypeData::SCORE_PLAYER, gnlayer->getScore(listen->getName()));
                    snd->dat.set(TypeData::SCORE_OPPONENT, gnlayer->getScore(listen->getOpponent()->getName()));
                }
                snd->dat.set(TypeData::LENGTH_PLAYER, listen->getLength());
                snd->dat.set(TypeData::LENGTH_OPPONENT, listen->getOpponent()->getLength());
            }

            if (eat)
                snd->dat.set(TypeData::POS_EAT, eat->getPosition() * sclFactor);

            if (bonus) {
                snd->dat.set(TypeData::POS_BONUS, bonus->getPosition() * sclFactor);
                snd->dat.set(TypeData::TYPE_BONUS, (int)bonus->getBonusType());
            }

            if (levelLayer)
                snd->dat.set(TypeData::TYPE_SHADER, levelLayer->getAttribMaskColor());

            if (statusShot.first) {
                snd->dat.set(TypeData::SHOT_BOMBA, statusShot.second);
                statusShot.first = false;
                log("SEND SHOT BOMBA");
            }
        }

        gameClient->setMsgToSend(snd->toStr());

    }, updateServer, NameSch);

}

void LocalPlayer::acceptRecvData(Node *ptr, TypeData type, ExperimentalSendData::Dat dat,
                                 const std::function<void (const std::string &)> &callback)
{
    if (!ptr)
        return;

    std::string &str = dat[type];

    if (!str.empty())
        callback(str);

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

        acceptRecvData(head, isRunningServer ? TypeData::POS_OPPONENT : TypeData::POS_PLAYER,
                       data, [&] (const std::string &source)
        {
            Vec2 pos = ExperimentalSendData::toVec2(source);
            if (pos.x > -1 && pos.y > -1) {
                currentPos = pos /= sclFactor;
                head->runAction(MoveTo::create(1.1f / UpdateRecver, currentPos));
            }
        });

        if (!isRunningServer) {

            acceptRecvData(eat, TypeData::POS_EAT, data, [=] (const std::string &source) {
                Vec2 pos = std::move(ExperimentalSendData::toVec2(source));
                pos /= sclFactor;
                if (eat->getPosition() != pos)
                    eat->setPosition(pos);
            });

            acceptRecvData(bonus, TypeData::POS_BONUS, data, [this] (const std::string &source) {
                Vec2 pos = ExperimentalSendData::toVec2(source);
                if (bonus->getPosition() != pos)
                    bonus->setPosition(pos);
            });

            acceptRecvData(bonus, TypeData::TYPE_BONUS, data, [this] (const std::string &source) {
                Bonus::TypeBonusMask type = (Bonus::TypeBonusMask)Value(source).asInt();
                if (bonus->getBonusType() != type)
                    bonus->setBonusType(type);
            });

            acceptRecvData(gnlayer, TypeData::SCORE_PLAYER, data, [this] (const std::string &source) {
                int score = Value(source).asInt();
                if (gnlayer->getScore(listen->getName()) != score)
                    gnlayer->setScore(score, listen->getName());
            });

            acceptRecvData(listen->getOpponent(), TypeData::SCORE_OPPONENT, data, [this] (const std::string &source) {
                int score = Value(source).asInt();
                if (gnlayer->getScore(listen->getOpponent()->getName()) != score)
                    gnlayer->setScore(score, listen->getOpponent()->getName());
            });

            acceptRecvData(levelLayer, TypeData::TYPE_SHADER, data, [this] (const std::string &source) {

                static int previousColor = -1;

                float shader = Value(source).asFloat();

                if (shader != previousColor) {

                    if (previousColor == -1) {
                        levelLayer->setAttribMaskColor(shader);
                    }

                    else {
                        levelLayer->shaderToSensitive(1, 1);

                        scheduleOnce([=](float){

                            levelLayer->setAttribMaskColor(shader);
                            levelLayer->shaderToSensitive(1, 0);

                        }, 1.1, "trans");
                    }
                }

                previousColor = shader;

            });

            acceptRecvData(listen, TypeData::LENGTH_PLAYER, data, [this] (const std::string &source) {

                if (!listen->getOpponent())
                    return;

                int len = Value(source).asInt();
                listen->getOpponent()->setRealLength(len);

            });

            acceptRecvData(listen->getOpponent(), TypeData::LENGTH_OPPONENT, data, [this] (const std::string &source) {

                int len = Value(source).asInt();
                listen->setRealLength(len);

            });

            acceptRecvData(listen, TypeData::SHOT_BOMBA, data, [this] (const std::string &source) {
                if (listen && listen->getOpponent()) {

                    log("SHOT BOMBA");

//                    callbackShotBomba({0, 0}, {400, 200});

//                    if (source == NamePlayer) {
//                        callbackShotBomba(listen->getPosition(), listen->getOpponent()->getPosition());
//                    }
//                    else {
//                        callbackShotBomba(listen->getOpponent()->getPosition(), listen->getPosition());
//                    }

                }
            });

        }
    };
}

void LocalPlayer::setEat(NormalEat *value)
{
    eat = value;
}

void LocalPlayer::setLevelLayer(LevelLayer *layer)
{
    levelLayer = layer;
}

void LocalPlayer::setIsRunningServer(bool value)
{
    isRunningServer = value;
}

void LocalPlayer::setBonus(Bonus *value)
{
    bonus = value;
}

void LocalPlayer::setGNLayer(GameNavigatorLayer *value)
{
    gnlayer = value;
}

std::pair<bool, std::string> LocalPlayer::getStatusShot() const
{
    return statusShot;
}

void LocalPlayer::setStatusShot(const std::pair<bool, std::string> &value)
{
    statusShot = value;
}

void LocalPlayer::setCallbackShotBomba(const std::function<void (const Vec2 &in, const Vec2 &to)> &value)
{
    callbackShotBomba = value;
}

LocalPlayer::~LocalPlayer()
{
    delete gameClient;
    gameClient = nullptr;
}
