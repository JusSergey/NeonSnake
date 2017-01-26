#ifndef __LEVELPRESENTATION_H__
#define __LEVELPRESENTATION_H__

#include "cocos2d.h"

class LevelPresentation : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    static void GoToLevelPresentation(int level, int bitmaskInits);

    // implement the "static create()" method manually
    CREATE_FUNC(LevelPresentation);

private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;

};

#endif // __LEVELPRESENTATION_H__
