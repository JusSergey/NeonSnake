#ifndef __PREGAMESETTINGLAYER_H__
#define __PREGAMESETTINGLAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Language.h"
#include <functional>
#include <map>

// class set colors players
class SwitchColorSnake : public cocos2d::Sprite {

    friend class PregameSettingLayer;

private:
    virtual bool init();
    CREATE_FUNC(SwitchColorSnake)
    void setCallbackSelectColor(const std::function<void(const cocos2d::Color3B &color, int tag)> &callback);

private:
    std::function<void(const cocos2d::Color3B &color, int tag)> callbackSelectColor;

}; // END CLASS SwitchColorSnake


// class for scroll levels.
class SwitchLevelGame : public cocos2d::ui::Button {

    friend class PregameSettingLayer;
private:

    virtual bool init();
    CREATE_FUNC(SwitchLevelGame)
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);
    typedef int _Tag;
    _Tag myTag;
    static std::map<_Tag, SwitchLevelGame *> globalObjects;

public:
    static void setSelectLevel(int selectLevel);

}; // END CLASS SwitchLevelGame


// main class for setting players
class PregameSettingLayer : public cocos2d::Layer
{
public:
    virtual bool init();

    CREATE_FUNC(PregameSettingLayer);

    // getPosition is not valid
    virtual void addChild(cocos2d::Node *child, int level);

public:
    void setCallbackNext(const std::function<void (Ref*)> &value);
    void setCallbackBackToMenu(const std::function<void (Ref*)> &value);
    void runToThisMenu();

private:
    enum {
        PlayerTag = 0xF,
        OpponentTag
    };

private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;

private:
    cocos2d::MenuItemLabel *itemNext;
    cocos2d::MenuItemLabel *itemBackToMenu;
    cocos2d::DrawNode *dnode;
    cocos2d::ui::TextField *fieldPlayer;
    cocos2d::ui::TextField *fieldOpponent;
    cocos2d::Label *labelPlayerTitle;
    cocos2d::Label *labelOpponentTitle;
    cocos2d::Label *labelPlayerColor;
    cocos2d::Label *labelOpponentColor;
    cocos2d::Sprite *blackBackground;
    cocos2d::Sprite *playerBackground;
    cocos2d::Sprite *opponentBackground;
    cocos2d::Sprite *scrollBackground;

private:
    typedef std::pair<cocos2d::Sprite*, bool> SwitchColorObj_t;
    typedef std::vector<SwitchColorObj_t> SwitchColorContainer_t;

    SwitchColorContainer_t swColorsPlayer1;
    SwitchColorContainer_t swColorsPlayer2;

private:
    void setSwitchColor(SwitchColorContainer_t &sw, const cocos2d::Color3B &color);

private:
    std::function<void(const cocos2d::Color3B &color, int tag)> getCallbackSelectColor();

private:
    void initItemNext();
    void initItemBackToMenu();
    void initMenu();
    void initDrawNode();
    void initBlackBackground();

    void initLabelSetting(cocos2d::Label* &rvLabel, const std::string &title, float procentY, const std::function<float(float)> &positioning);
    void initColors(SwitchColorContainer_t &sw, const std::function<float(float)> &positioning, int tag);
    void initTextFields(const std::function<float(float)> &positioning, cocos2d::ui::TextField* &field, const std::string &name);

    std::vector<SwitchLevelGame *> levels;
    void initScrollViewLevels();

public:
    void setLanguageLabels(Locale locale);

}; // END CLASS PregameSettingLayer

#endif // __PREGAMESETTINGLAYER_H__
