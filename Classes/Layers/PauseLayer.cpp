#include "PauseLayer.h"
#include "Scenes/MenuScene.h"
#include "Jus.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool PauseLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if(!Layer::init())
        return false;

    log("initialize PauseLayer...");

    callbackHome    = [](){};
    callbackRestart = [](){};
    callbackResume  = [](){};

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    initPauseMenu();

    return true;
}

void PauseLayer::setCallbackHome(const std::function<void ()> &value)
{
    callbackHome = value;
}

void PauseLayer::setCallbackRestart(const std::function<void ()> &value)
{
    callbackRestart = value;
}

void PauseLayer::setCallbackResume(const std::function<void()> &value)
{
    callbackResume = value;
}

void PauseLayer::setVisible(bool visible)
{
    Layer::setVisible(visible);

    for (Node *child : getChildren())
        child->setVisible(visible);

}

void PauseLayer::initPauseMenu()
{
    initShadow();
    initButtonResume();
    initButtonRestart();
    initButtonHome();

    auto menu = Menu::create(itemHome, itemRestart, itemResume, nullptr);

    menu->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    menu->setPosition(Vec2::ZERO);

    addChild(menu);
}

void PauseLayer::initButtonResume()
{
    auto sprite = Sprite::create("ButtonResume.png");

    itemResume = MenuItemSprite::create(sprite, sprite);

    itemResume->setOpacity(0xff * .30);

    itemResume->setCallback([this](Ref*){ clickResume(); });

    itemResume->setPosition(Jus::getDisplayPoint(0.5, 0.5));

}

void PauseLayer::initButtonRestart()
{
    auto sprite = Sprite::create("Restart.png");

    itemRestart = MenuItemSprite::create(sprite, sprite);

    itemRestart->setOpacity(0xff * .30);

    itemRestart->setCallback([this](Ref*){ clickRestart(); });

    itemRestart->setPosition(Jus::getDisplayPoint(0.25, 0.5));
}

void PauseLayer::initButtonHome()
{
    auto sprite = Sprite::create("Home.png");

    itemHome = MenuItemSprite::create(sprite, sprite);

    itemHome->setOpacity(0xff * .30);

    itemHome->setCallback([this](Ref*) { clickHome(); });

    itemHome->setPosition(Jus::getDisplayPoint(0.75, 0.5));
}

void PauseLayer::initShadow()
{
    shadow = DrawNode::create();
    shadow->setPosition(Vec2::ZERO);
    addChild(shadow);

    shadow->drawSolidRect(Vec2::ZERO, visibleSize, Color4F(0, 0, 0, 0.4));
}

void PauseLayer::clickResume()
{
    callbackResume();
    setVisible(false);
}

void PauseLayer::clickRestart()
{
    callbackRestart();
}

void PauseLayer::clickHome()
{
    callbackHome();
}
