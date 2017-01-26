#ifndef __PLAYERDATA_H__
#define __PLAYERDATA_H__

#include "cocos2d.h"

using cocos2d::Vec2;

struct PlayerData {

    Vec2 opponentPos;
    Vec2 bonusPos;
    Vec2 eatPos;
    int shader;
    void operator = (const std::string &buff);
    void operator = (const char *buff);
    PlayerData() : opponentPos(-1, -1), eatPos(-1, -1) {}
    PlayerData(const std::string &source);
    PlayerData(const char *data);
    std::string toString() const;
    static Vec2 stringToVec2(const std::string &str);

};


#endif // __PLAYERDATA_H__

