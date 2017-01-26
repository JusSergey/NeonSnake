#ifndef __MENUSCENE_H__
#define __MENUSCENE_H__

#include "cocos2d.h"
#include "Layers/PregameSettingLayer.h"
#include "Layers/PregameSwitchTypeGameLayer.h"
#include "Layers/NetworkSettingLayer.h"
#include "GameView.h"

class MenuScene : public GameView
{
public:
    virtual bool init();
    static cocos2d::Scene* createScene(int bitmask, int bitmaskGN = InitNo);
    static MenuScene* create(int bitmask, int bitmaskGN);
    static void GoToMenuScene(int bitmask = InitGameNavigator | InitCamera | InitBot | InitEat, int bitmaskGN = InitGNSoundMenu);

private:
    cocos2d::Camera   *camera;
    cocos2d::DrawNode *draw;
    cocos2d::Menu     *menu;
    cocos2d::Sprite   *filter;

    cocos2d::MenuItemLabel *itemStart;
    cocos2d::MenuItemLabel *itemSurvival;
    cocos2d::MenuItemLabel *itemLocal;
    cocos2d::MenuItemLabel *itemExit;

    PregameSettingLayer        *pregameSettingLayer;
    PregameSwitchTypeGameLayer *pregameSwitchTypeGameLayer;
    NetworkSettingLayer        *networkSettingLayer;

    mutable bool isClickStartServer;

private:
    void clickStart();
    void clickSurvival();
    void clickLocal();
    void clickExit();

private:
    virtual std::function<void()> getCallbackMusicButton() const;
            std::function<void(Ref*)> getCallbackStartServer() const;

private:
    void initCamera();
    void initDrawNode();
    void initTouches();
    void initMenu();
    void initSnow();
    void initPregameSettingLayer();
    void initPregameSwitchTypeGame();
    void initNetworkSettingLayer();
    cocos2d::Label *createLabel(const std::string &text) const;

};

#endif // __MENUSCENE_H__
