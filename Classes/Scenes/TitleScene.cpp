#include "TitleScene.h"
#include "MenuScene.h"
#include "Sound/Audio.h"
#include <thread>
#include "Jus.h"

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

    title = Sprite::create("title.png");
    title->setPosition(visibleSize / 2);
    addChild(title);

    initLabelsPresent();
    initLabelsDev();

    return true;
}

void TitleScene::GoToTitleScene()
{
    Director::getInstance()->replaceScene(TransitionFade::create(1, TitleScene::createScene()));

}

void TitleScene::initLabelsDev()
{
    devVK = Jus::createLabelSystem("dev: vk.com/cmd_exe", "fonts/Bicubik.ttf", 16);

    devVK->setOpacity(0);
    devVK->setAdditionalKerning(3);
    devVK->setPosition(visibleSize.width / 2, devVK->getContentSize().height);
    addChild(devVK);

    devVK->runAction(FadeIn::create(1));
}

void TitleScene::initLabelsPresent()
{
    Label *label = Jus::createLabelTTF("The Glow Snake", "fonts/Bicubik.ttf", 64);
    const float startx = visibleSize.width / 2 - label->getContentSize().width / 2;

    Label *the = Jus::createLabelTTF("The ", "fonts/Bicubik.ttf", 64);
    Label *glow = Jus::createLabelTTF("Glow ", "fonts/Bicubik.ttf", 64);
    Label *snake = Jus::createLabelTTF("Snake", "fonts/Bicubik.ttf", 64);

    float offset = startx;

    for (Label *l : {the, glow, snake}) {
        l->enableShadow(Color4B::BLUE);
        l->setOpacity(0);
        l->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        l->setPosition(offset, visibleSize.height / 2);
        offset += l->getContentSize().width;
        addChild(l);
    }

    static const char *sch = "schLbls";

    Label* lbls[] {the, glow, snake};

    static int index;
    index = 0;

    schedule([=, &index](float){

        if (index > 2) {
            unschedule(sch);

            scheduleOnce([=](float){
                auto fade = FadeOut::create(1);
                the->runAction(fade);
                glow->runAction(fade->clone());
                snake->runAction(fade->clone());
                devVK->runAction(fade->clone());
                title->runAction(fade->clone());
            }, 1.5, "schonce");

            return;
        }

        lbls[index++]->runAction(FadeIn::create(0.7));

    }, 0.3, sch);

}

void TitleScene::onEnterTransitionDidFinish()
{
    scheduleOnce([this](float){
        MenuScene::GoToMenuScene();
    }, 4, "onceTransition");
}
