#include "SendData.h"

USING_NS_CC;

std::string SendData::namePlayer;
std::string SendData::nameOpponent;
cocos2d::Vec2 SendData::posPlayer;
cocos2d::Vec2 SendData::posOpponent;
cocos2d::Vec2 SendData::posEat;
cocos2d::Vec2 SendData::posBonus;
bool SendData::visibleBonus(false);
int SendData::scorePlayer(0);
int SendData::scoreOpponent(0);
int SendData::currentTime(0);
int SendData::directTime(0);
int SendData::typeShader(0);

std::string SendData::toString()
{

}

ExperimentalSendData::string ExperimentalSendData::toStr() const
{
    ExperimentalSendData::string str;

    for (const auto &i : dat)
        str += i.first + '=' + i.second + '\n';

    return str;
}


Vec2 ExperimentalSendData::toVec2(const std::string &value)
{
    auto posSeparator = value.find(' ');
    auto p1 = value.substr(0, posSeparator);
    auto p2 = value.substr(posSeparator+1, value.length());

    return cocos2d::Vec2( Value(p1).asFloat(), Value(p2).asFloat() );
}
