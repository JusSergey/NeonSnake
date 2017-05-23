#ifndef __TITLESCENE_H__
#define __TITLESCENE_H__

#include "cocos2d.h"
#include <thread>

using namespace std;


class TitleScene : public cocos2d::Layer
{
    TitleScene();
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(TitleScene)

    static void GoToTitleScene();

private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::Label *devVK;
    cocos2d::Sprite *title;

private:
    void initLabelsDev();
    void initLabelsPresent();

protected:
    virtual void onEnterTransitionDidFinish();

};

#endif // __TITLESCENE_H__
