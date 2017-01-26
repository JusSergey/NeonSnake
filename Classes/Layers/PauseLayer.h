#ifndef __PAUSELAYER_H__
#define __PAUSELAYER_H__

#include "cocos2d.h"
#include <functional>

class PauseLayer : public cocos2d::Layer
{
public:
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(PauseLayer);

private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;

private:
    std::function<void()> callbackResume;
    std::function<void()> callbackRestart;
    std::function<void()> callbackHome;
    cocos2d::MenuItemSprite *itemResume;
    cocos2d::MenuItemSprite *itemRestart;
    cocos2d::MenuItemSprite *itemHome;
    cocos2d::DrawNode *shadow;

public:
    virtual void setVisible(bool visible);

public:
    void setCallbackResume(const std::function<void()> &value);
    void setCallbackRestart(const std::function<void ()> &value);
    void setCallbackHome(const std::function<void ()> &value);

private:
    void initPauseMenu();
    void initButtonResume();
    void initButtonRestart();
    void initButtonHome();
    void initShadow();

private:
    void clickResume();
    void clickRestart();
    void clickHome();

};

#endif // __PAUSELAYER_H__

