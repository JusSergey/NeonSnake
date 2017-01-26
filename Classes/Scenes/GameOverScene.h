#ifndef __GAMEOVERSCENE_H__
#define __GAMEOVERSCENE_H__

#include "cocos2d.h"

class GameOverScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    static void GoToGameOver();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(GameOverScene);

private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;

};

#endif // __GAMEOVERSCENE_H__
