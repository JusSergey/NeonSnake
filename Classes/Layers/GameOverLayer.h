#ifndef __GAMEOVERLAYER_H__
#define __GAMEOVERLAYER_H__

#include "cocos2d.h"

class GameOverLayer : public cocos2d::Sprite
{
public:
    virtual bool init();
    CREATE_FUNC(GameOverLayer)

private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;

    cocos2d::Label *labelWin;
    cocos2d::Label *labelScore;
    cocos2d::Label *labelBonus;
    cocos2d::Label *labelEat;

    std::string _name;
    int _score;
    int _bonus;
    int _eat;

private:
    void initLabels();

public:
    void setNameWin(const std::string &name);
    void setScore(int count);
    void setBonus(int count);
    void setEat(int count);

    inline const std::string &getNameWin() const { return _name; }
    inline int getScore() const { return _score; }
    inline int getBonus() const { return _bonus; }
    inline int getEat()   const { return _eat;   }

};

#endif // __GAMEOVERLAYER_H__
