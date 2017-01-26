#include "GameOverScene.h"
#include "MenuScene.h"
#include "Sound/Audio.h"

USING_NS_CC;

Scene* GameOverScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'Layer' is an autorelease object
    auto layer = GameOverScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void GameOverScene::GoToGameOver()
{
    Director::getInstance()->replaceScene(TransitionFade::create(3, GameOverScene::createScene()));
}

// on "init" you need to initialize your instance
bool GameOverScene::init()
{
    //////////////////////////////
    // 1. super init first
    if(!Layer::init())
        return false;

    log("initialize GameOverScene...");

    Audio::getInstance()->stopBackgroundMusic();

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    LabelTTF *label = LabelTTF::create("Ops. Game over.. :(", "fonts/Bicubik.ttf", 64);
    label->setPosition(visibleSize / 2);
    addChild(label);

    scheduleOnce([this](float){
        MenuScene::GoToMenuScene();
    }, 5, "gameover");

    return true;
}
