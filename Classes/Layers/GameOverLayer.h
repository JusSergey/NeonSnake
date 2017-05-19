#ifndef __GAMEOVERLAYER_H__
#define __GAMEOVERLAYER_H__

#include "cocos2d.h"
#include <functional>

class GameOverLayer : public cocos2d::Sprite
{
public:
    enum ID_SNAKE : int { FIRST = 0, SECOND = 1, NO_WINS = 2 };

public:
    virtual bool init();
    CREATE_FUNC(GameOverLayer)

private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;

    cocos2d::Label *labelTitle;
    cocos2d::Label *labelName[2];
    cocos2d::Label *labelScore[2];
    cocos2d::Label *labelBonus[2];
    cocos2d::Label *labelEat[2];

    cocos2d::MenuItemSprite *itemRestart;
    cocos2d::MenuItemSprite *itemHome;
    cocos2d::MenuItemSprite *itemNext;
    cocos2d::Menu *menu;

    std::string _name[2];
    int _score[2];
    int _bonus[2];
    int _eat[2];

private:
    void initLabels();
    void initSeparator();
    void initMenu();
    void initSpritesBonus();

    void runFadeInMainBorder(std::function<void()> callfuntOfEnded);

public:
    void showDanceWin(ID_SNAKE id);
    void showFireworks();
    void setScore(ID_SNAKE id, int count);
    void setBonus(ID_SNAKE id, int count);
    void setEat(ID_SNAKE id, int count);
    void setSnakeName(ID_SNAKE id, const std::string &name);
    void setCallbackRestart(const std::function<void()> &callback);
    void setCallbackHome(const std::function<void()> &callback);
    void setCallbackNext(const std::function<void()> &callback);

    inline const std::string &getNameWin(ID_SNAKE id) const { return _name[id]; }
    inline int getScore(ID_SNAKE id) const { return _score[id]; }
    inline int getBonus(ID_SNAKE id) const { return _bonus[id]; }
    inline int getEat(ID_SNAKE id)   const { return _eat[id];   }

private:
    class BonusSprite : cocos2d::Sprite
    {
        cocos2d::Label *labelCountBonusses = nullptr;
        void initLabel();
    public:
        void setCountBonusses(int n);
        void addCountBonusses(int n);

    };

};

#endif // __GAMEOVERLAYER_H__
