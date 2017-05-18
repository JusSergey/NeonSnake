#ifndef __GAMENAVIGATORLAYER__
#define __GAMENAVIGATORLAYER__

#include "cocos2d.h"
#include "Constant.h"
#include "PauseLayer.h"
#include "Language.h"

/*
1.) you: [w*0+f, h*0.95] [w*1-f, h*0.95]
2.) bot: [w*0.5, h*0+f]  [w*0.5, h*1-f]
*/

class GameNavigatorLayer : public cocos2d::Layer
{
public:
    virtual bool init();
    static GameNavigatorLayer* create(GameMode mode, int bitmaskInits = InitGameNavigatorAll);
//    CREATE_FUNC(GameNavigatorLayer);

private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::DrawNode *node;
    cocos2d::MenuItemSprite *spriteResume;
    cocos2d::MenuItemSprite *buttonSound;
    cocos2d::Action *actionOpponent;
    cocos2d::Action *actionPlayer;

private:
    bool visibleSoundMenu;
    float posLabelEffectY;
    float posLabelMusicY;
    PauseLayer *pauseLayer;
    cocos2d::MenuItemLabel *labelEffectItemMenu;
    cocos2d::MenuItemLabel *labelMusicItemMenu;
    cocos2d::Label *labelScoreBot;
    cocos2d::Label *labelScorePlayer;
    cocos2d::Label *labelTimer;
    std::function<void()> callbackPause;
    std::function<void()> callbackTimeout;
    std::function<void()> callbackButtonEffect;
    std::function<void()> callbackButtonMusic;
    std::function<void()> callbackButtonSoundMenu;

private:
    int timeInSeconds;
    int stepSeconds;
    GameMode mode;
    int bitmaskInits;

public:
    int countScorePlayer;
    int countScoreBot;

private:
    void initAcceleration();
    void initLabels();
    void initLabel(cocos2d::Label *&rPtr, const cocos2d::Vec2 &pos, const cocos2d::Vec2 &anchor, const std::string &text);
    void initTimer();
    void initPauseButton();
    void initPausedLayer();
    void initSoundButton();


private:
    void clickButtonEffect();
    void clickButtonMusic();

private:
    cocos2d::Action *createAction();

public:
    void setCallbackPause   (const std::function<void()> &call);
    void setCallbackResume  (const std::function<void()> &call);
    void setCallbackTimeout (const std::function<void()> &call);

    void setCallbackButtonEffect(const std::function<void()> &call);
    void setCallbackButtonMusic (const std::function<void()> &call);

    void setCallbackButtonSoundMenu(const std::function<void ()> &value);

    void setCallbackRestart(const std::function<void ()> &value);
    void setCallbackHome   (const std::function<void ()> &value);

    void setLanguageLabels(Locale locale);

    void setVisibleLabelPlayer(bool visible) { if(labelScoreBot) labelScoreBot->setVisible(visible); }
    void setVisibleLabelOpponent(bool visible) {  if(labelScorePlayer) labelScorePlayer->setVisible(visible);}

public:
    void showPauseLayer ();
    void hideLabels     (float duration);
    void hideContextMenu(float lastTime);
    void addScores    (int value, const std::string &nameSnake);
    void setScore     (int value, const std::string &nameSnake);
    void setTimeLevel (int secons);
    int  getScore     (const std::string &nameSnake) const;

    void setVisibleContentSoundMenu(bool on, bool noAction = false);
    inline bool isVisibleSoundMenu() { return visibleSoundMenu; }

    int getStepSeconds() const;
    void setStepSeconds(int value);
};

#endif // __GAMENAVIGATORLAYER__
