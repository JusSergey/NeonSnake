#include "GameNavigatorLayer.h"
#include "Constant.h"
#include "Data/DataSetting.h"
#include "Sound/Audio.h"
#include "Scenes/MenuScene.h"

#define TIME_VISIBLE_CONTEXT_SOUND_MENU 1.5f

USING_NS_CC;

#define INIT_ALL ((bitmaskInits ^ InitGameNavigator) & InitGameNavigatorAll)

using StringUtils::toString;

// on "init" you need to initialize your instance
bool GameNavigatorLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if(!Layer::init())
        return false;

    stepSeconds = 1;

    log("Start init GN");

    pauseLayer = nullptr;
    labelScoreBot = nullptr;
    labelScorePlayer = nullptr;
    labelEffectItemMenu = nullptr;
    labelMusicItemMenu = nullptr;
    labelTimer = nullptr;

    callbackPause   = [](){};
    callbackTimeout = [](){};

    callbackButtonEffect    = [](){};
    callbackButtonMusic     = [](){};
    callbackButtonSoundMenu = [](){};

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    initLabels();

    if (INIT_ALL || bitmaskInits & InitGNPauseButton) {
        log("initGNPAUSE");
        initPauseButton();
        initPausedLayer();
    }

    if (INIT_ALL || bitmaskInits & InitGNSoundMenu)
        initSoundButton();


    return true;
}

GameNavigatorLayer *GameNavigatorLayer::create(GameMode mode, int bitmaskInits)
{
    GameNavigatorLayer *pRet = new(std::nothrow) GameNavigatorLayer();
    if (pRet) {
        pRet->mode = mode;
        pRet->bitmaskInits = bitmaskInits;
    }
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

void GameNavigatorLayer::setCallbackButtonSoundMenu(const std::function<void ()> &value)
{
    callbackButtonSoundMenu = value;
}

void GameNavigatorLayer::setCallbackHome(const std::function<void ()> &value)
{
    if (pauseLayer)
        pauseLayer->setCallbackHome(value);
}

void GameNavigatorLayer::hideContextMenu(float lastTime)
{
    static const char *nameSch = "AutoHide";

    unschedule(nameSch);

    scheduleOnce([this](float){
        if (visibleSoundMenu)
            setVisibleContentSoundMenu(false);
    }, lastTime, nameSch);
}

void GameNavigatorLayer::setCallbackRestart(const std::function<void ()> &value)
{
    if (pauseLayer)
        pauseLayer->setCallbackRestart(value);
}

int GameNavigatorLayer::getStepSeconds() const
{
    return stepSeconds;
}

void GameNavigatorLayer::setStepSeconds(int value)
{
    stepSeconds = value;
}

void GameNavigatorLayer::initAcceleration()
{
    Label* debug = Label::createWithTTF("-", "fonts/Bicubik.ttf", 18);
    debug->setPosition(visibleSize / 2);
    addChild(debug, 10);

    Device::setAccelerometerInterval(1.f / 2);
    Device::setAccelerometerEnabled(true);

    auto listener = EventListenerAcceleration::create([=](Acceleration *accel, Event*){
        std::string
        str  = "x: " + toString(accel->x) + "\n";
        str += "y: " + toString(accel->y) + "\n";
        str += "z: " + toString(accel->z) + "\n";
        debug->setString(str);
    });
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameNavigatorLayer::initLabels()
{
    log("Init Labels");

    if (INIT_ALL || bitmaskInits & InitGNPlayer1Label)
        initLabel(labelScorePlayer,
                  Vec2(AlignLabel, visibleSize.height * 0.95),
                  Vec2::ANCHOR_MIDDLE_LEFT,
                  UserData::playerName + ": 0");

    if (INIT_ALL || bitmaskInits & InitGNPlayer2Label)
        initLabel(labelScoreBot,
                  Vec2(visibleSize.width - AlignLabel, visibleSize.height * 0.95),
                  Vec2::ANCHOR_MIDDLE_RIGHT,
                  UserData::opponentName + ": 0");

    if (INIT_ALL || bitmaskInits & InitGNTimer)
        initTimer();

}

void GameNavigatorLayer::initLabel(LabelTTF *&rPtr, const Vec2 &pos, const Vec2 &anchor, const std::string &text)
{
    log("Init Label");
    rPtr = LabelTTF::create(text, "", SizeFont);
    rPtr->setAnchorPoint(anchor);
    rPtr->setPosition(pos);
    addChild(rPtr);
}

void GameNavigatorLayer::initTimer()
{
    log("Init Timer");
    initLabel(labelTimer, Vec2(visibleSize.width / 2, visibleSize.height * 0.95), Vec2::ANCHOR_MIDDLE, "");
    labelTimer->setOpacity(0xff * .75);
}

void GameNavigatorLayer::initPauseButton()
{
    log("Init Pause Button");
    Sprite *button = Sprite::create("ButtonPause.png");
    button->setOpacity(0xff * .25);

    auto callback = [this](Ref*) -> void {
        pauseLayer->setVisible(true);
        callbackPause();
    };

    MenuItemSprite *spriteButton = MenuItemSprite::create(button, button, button, callback);
    Menu *menu = Menu::create(spriteButton, nullptr);

    menu->setPosition(Vec2::ZERO);

    Size sz = button->getContentSize();

    spriteButton->setPosition(visibleSize.width - sz.width, sz.height);

    addChild(menu, LScore);
}

void GameNavigatorLayer::initPausedLayer()
{
    log("Init Pause Layer");
    pauseLayer = PauseLayer::create();
    addChild(pauseLayer, LPause);

    pauseLayer->setVisible(false);
}

void GameNavigatorLayer::initSoundButton()
{
    log("Init Sound Button");
    Sprite *soundSprite = Sprite::create("Volume.png");

    buttonSound = MenuItemSprite::create(soundSprite, soundSprite, soundSprite);
    buttonSound->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    Size sz = buttonSound->getContentSize();

    buttonSound->setPosition(sz.width / 2 + AlignLabel, sz.height / 2 + AlignLabel - 5);

    buttonSound->setOpacity(0xff * 0.25);

    // init labels
    auto labelEffect = Label::createWithTTF("Effect", "fonts/Bicubik.ttf", 24);
    auto labelMusic  = Label::createWithTTF("Music", "fonts/Bicubik.ttf", 24);
    labelEffect->setAdditionalKerning(3);
    labelMusic->setAdditionalKerning(3);
    labelEffectItemMenu = MenuItemLabel::create(labelEffect, [this](Ref*){ clickButtonEffect(); });
    labelMusicItemMenu  = MenuItemLabel::create(labelMusic,  [this](Ref*){ clickButtonMusic();  });


    labelEffectItemMenu->setOpacity(0);
    labelEffectItemMenu->setVisible(false);
    labelMusicItemMenu->setOpacity(0);
    labelMusicItemMenu->setVisible(false);
    visibleSoundMenu = false;

    float offsetX = buttonSound->getContentSize().width / 2;

    labelEffectItemMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    labelMusicItemMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);

    //!
    posLabelEffectY = buttonSound->getPositionY() + labelEffectItemMenu->getContentSize().height + 20;
    posLabelMusicY  = posLabelEffectY + labelMusicItemMenu->getContentSize().height + 10;

    labelEffectItemMenu->setPosition(buttonSound->getPosition() - Vec2(offsetX, 0));

    labelMusicItemMenu->setPosition(buttonSound->getPosition() - Vec2(offsetX, 0));
    //

    buttonSound->setCallback([this](Ref*){
        log("Callback Button Sound");
        if (!Director::getInstance()->isPaused()) {
            setVisibleContentSoundMenu(!isVisibleSoundMenu());
            log("press button volume");
        }
        callbackButtonSoundMenu();
    });

    labelEffectItemMenu->setColor((UserData::playingEffectSound)     ? Color3B::WHITE : Color3B(80, 80, 80));
    labelMusicItemMenu ->setColor((UserData::playingBackgroundMusic) ? Color3B::WHITE : Color3B(80, 80, 80));

    Menu *menu  = Menu::create(buttonSound, labelEffectItemMenu, labelMusicItemMenu, nullptr);
    menu->setPosition(Vec2::ZERO);
    menu->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

    addChild(menu, LScore);

}

void GameNavigatorLayer::clickButtonEffect()
{
    log("Clicked Button Effect");

    hideContextMenu(TIME_VISIBLE_CONTEXT_SOUND_MENU);

    UserData::playingEffectSound = !UserData::playingEffectSound;

    labelEffectItemMenu->setColor(UserData::playingEffectSound ? Color3B::WHITE : Color3B(80, 80, 80));

    DataSetting::save();

    auto actMan = Director::getInstance()->getActionManager();

    if (actMan->getNumberOfRunningActionsInTarget(labelEffectItemMenu) || actMan->getNumberOfRunningActionsInTarget(labelMusicItemMenu))
        setVisibleContentSoundMenu(true);

    callbackButtonEffect();
}

void GameNavigatorLayer::clickButtonMusic()
{
    log("Clicked Button Sound");

    hideContextMenu(TIME_VISIBLE_CONTEXT_SOUND_MENU);

    UserData::playingBackgroundMusic = !UserData::playingBackgroundMusic;

    labelMusicItemMenu->setColor(UserData::playingBackgroundMusic ? Color3B::WHITE : Color3B(80, 80, 80));

    DataSetting::save();

    callbackButtonMusic();
}

Action *GameNavigatorLayer::createAction()
{
    return Sequence::create(ScaleTo::create(0.2, 2), ScaleTo::create(0.2, 1), nullptr);
}

void GameNavigatorLayer::addScores(int value, const std::string &nameSnake)
{

    if (nameSnake == NameBotOrOpponent)
        setScore(countScoreBot + value, nameSnake);

    else if (nameSnake == NamePlayer)
        setScore(countScorePlayer + value, nameSnake);

}

void GameNavigatorLayer::setScore(int value, const std::string &nameSnake)
{
    if (nameSnake == NameBotOrOpponent) {
        if (labelScoreBot) {
            labelScoreBot->setString(UserData::opponentName + ": " + toString(value));
            labelScoreBot->stopAllActions();
            labelScoreBot->runAction(createAction());
            countScoreBot = value;
        }
    }
    else if (nameSnake == NamePlayer) {
        if (labelScorePlayer) {
            labelScorePlayer->setString(UserData::playerName + ": " + toString(value));
            labelScorePlayer->stopAllActions();
            labelScorePlayer->runAction(createAction());
            countScorePlayer = value;
        }
    }
}

void GameNavigatorLayer::setTimeLevel(int secons)
{
    log("Set Time Level");
    if (labelTimer == nullptr)
        return;

    timeInSeconds = secons;

    const char *nameSchedule = "timerUpdateTimer";
    unschedule(nameSchedule);

    schedule([=](float){

        if (mode == GameMode::Default && timeInSeconds <= 0) {
            unschedule(nameSchedule);
            callbackTimeout();
        }

        else {
            timeInSeconds += stepSeconds;
            std::string minute = (timeInSeconds >= 60 ? (toString(timeInSeconds / 60)) : "");
            std::string second = (timeInSeconds % 60 < 10 ? "0" : "") + toString(timeInSeconds % 60);

            labelTimer->setString((minute.empty() ? "" : minute + ":") + second);
        }

    }, 1, nameSchedule);

}

int GameNavigatorLayer::getScore(const std::string &nameSnake) const
{
    if (nameSnake == NameBotOrOpponent) return countScoreBot;
    if (nameSnake == NamePlayer) return countScorePlayer;

    log("Not valid name snake: '%s'. Because return -1.", nameSnake.c_str());

    return -1;
}

void GameNavigatorLayer::setCallbackPause(const std::function<void ()> &call)
{
    log("setCallbackPause");
    callbackPause = call;
}

void GameNavigatorLayer::setCallbackResume(const std::function<void ()> &call)
{
    log("setCallbackResume");
    if (pauseLayer)
        pauseLayer->setCallbackResume(call);
}

void GameNavigatorLayer::setCallbackTimeout(const std::function<void ()> &call)
{
    log("setCallbackTimeout");
    callbackTimeout = call;
}

void GameNavigatorLayer::setCallbackButtonEffect(const std::function<void ()> &call)
{
    log("setCallbackButtonEffect");
    callbackButtonEffect = call;
}

void GameNavigatorLayer::setCallbackButtonMusic(const std::function<void ()> &call)
{
    log("setCallbackButtonMusic");
    callbackButtonMusic = call;
}

void GameNavigatorLayer::setVisibleContentSoundMenu(bool on, bool noAction)
{
    log("SetVisibleContentSoundMenu");


    auto actMan = Director::getInstance()->getActionManager();

    if ((actMan->getNumberOfRunningActionsInTarget(labelEffectItemMenu) ||
        actMan->getNumberOfRunningActionsInTarget(labelMusicItemMenu))  &&
        on != visibleSoundMenu)
        return;

    visibleSoundMenu = on;

    if (!labelEffectItemMenu || !labelMusicItemMenu)
        return;

    float offsetX = buttonSound->getContentSize().width / 2;

    labelEffectItemMenu->stopAllActions();
    labelMusicItemMenu->stopAllActions();

    if (on) {
        labelEffectItemMenu->setVisible(true);
        labelMusicItemMenu ->setVisible(true);

        if (noAction){
            labelEffectItemMenu->setOpacity(0xff);
            labelMusicItemMenu ->setOpacity(0xff);

            labelEffectItemMenu->setPosition(buttonSound->getPositionX() - offsetX, posLabelEffectY);
            labelMusicItemMenu ->setPosition(buttonSound->getPositionX() - offsetX, posLabelMusicY);
        }

        else {
            auto moveEffect = MoveTo::create(0.2, Vec2(buttonSound->getPositionX() - offsetX, posLabelEffectY));
            auto moveMusic  = MoveTo::create(0.2, Vec2(buttonSound->getPositionX() - offsetX, posLabelMusicY));

            labelEffectItemMenu->runAction(Spawn::create(moveEffect, FadeIn::create(0.2), nullptr));
            labelMusicItemMenu ->runAction(Spawn::create(moveMusic , FadeIn::create(0.2),  nullptr));

            hideContextMenu(TIME_VISIBLE_CONTEXT_SOUND_MENU);
        }

    }
    else {

        if (noAction) {
            labelEffectItemMenu->setOpacity(0x00);
            labelMusicItemMenu ->setOpacity(0x00);
            labelEffectItemMenu->setVisible(false);
            labelMusicItemMenu ->setVisible(false);

            labelEffectItemMenu->setPosition(buttonSound->getPositionX() - offsetX, buttonSound->getPositionY());
            labelMusicItemMenu ->setPosition(buttonSound->getPositionX() - offsetX, buttonSound->getPositionY());
        }

        else {
            auto move = MoveTo ::create(0.2, Vec2(buttonSound->getPositionX() - offsetX, buttonSound->getPositionY()));
            auto fade = FadeOut::create(0.2);

            auto callEffect = CallFunc::create([=](){ labelEffectItemMenu->setVisible(false); });
            auto callMusic  = CallFunc::create([=](){ labelMusicItemMenu ->setVisible(false); });

            auto actionEffect = Sequence::create(Spawn::create(move,          fade,          nullptr), callEffect, nullptr);
            auto actionMusic  = Sequence::create(Spawn::create(move->clone(), fade->clone(), nullptr), callMusic , nullptr);

            labelEffectItemMenu->runAction(actionEffect);
            labelMusicItemMenu ->runAction(actionMusic);
        }
    }
}
