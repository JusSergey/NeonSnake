#ifndef __SENDDATA_H__
#define __SENDDATA_H__

#include "cocos2d.h"

class ExperimentalSendData {

public:
    static ExperimentalSendData *getInst() {
        static auto *pointer = new ExperimentalSendData;
        return pointer;
    }

    typedef std::string string;

    std::map<string, string> dat;

    string toStr() const;

    cocos2d::Vec2 toVec2(const std::string &value);

};

class SendData
{
private:
    SendData() = delete;
   ~SendData() = delete;

public:
    static std::string namePlayer;
    static std::string nameOpponent;
    static cocos2d::Vec2 posPlayer;
    static cocos2d::Vec2 posOpponent;
    static cocos2d::Vec2 posEat;
    static cocos2d::Vec2 posBonus;
    static bool visibleBonus;
    static int scorePlayer;
    static int scoreOpponent;
    static int currentTime;
    static int directTime;
    static int typeShader;

public:
    std::string toString();

};

#endif // __SENDDATA_H__
