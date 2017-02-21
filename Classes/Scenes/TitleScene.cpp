#include "TitleScene.h"
#include "MenuScene.h"
#include "Sound/Audio.h"
#include <thread>
//#include <future>

//std::future<void> t_fkfldsaljcsdcl;

USING_NS_CC;

void fpreload(){
    log("call preload");
    Audio::getInstance()->preload();
}



TitleScene::TitleScene() : /*PreloadSounds(std::async(std::launch::async, fpreload)),*/ Layer()
{
    std::thread(fpreload).detach();
}

Scene* TitleScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'Layer' is an autorelease object

    auto layer = TitleScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TitleScene::init()
{
    //////////////////////////////
    // 1. super init first
    if(!Layer::init())
        return false;

    log("initialize TitleScene...");

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    auto spriteSername = Sprite::create("title.png");
    spriteSername->setPosition(visibleSize / 2);
    addChild(spriteSername);

    return true;
}

void TitleScene::GoToTitleScene()
{
    Director::getInstance()->replaceScene(TransitionFade::create(1, TitleScene::createScene()));

}

void TitleScene::onEnterTransitionDidFinish()
{
    scheduleOnce([this](float){

//        Audio::getInstance()->preload();
        MenuScene::GoToMenuScene();
    }, 1, "onceTransition");
}
