#include "LevelPresentation.h"
#include "Constant.h"
#include "GameView.h"
#include "Sound/Audio.h"

USING_NS_CC;

static int levelLoad = 0;
static int bitmaskInits = InitNo;

Scene* LevelPresentation::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'Layer' is an autorelease object

    auto layer = LevelPresentation::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LevelPresentation::init()
{
    //////////////////////////////
    // 1. super init first
    if(!Layer::init())
        return false;

    log("initialize LevelPresentation...");

    Audio::getInstance()->stopBackgroundMusic();

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    Label *label = Label::createWithSystemFont(Statuces[levelLoad-1], "monospace", 48);
    label->setPosition(visibleSize / 2);
    addChild(label);

    scheduleOnce([this](float){
        GameView::GoToGameView(levelLoad, bitmaskInits);
    }, 6, "gameview");

    return true;
}

void LevelPresentation::GoToLevelPresentation(int level, int bitmaskInits)
{
    ::levelLoad = level;
    ::bitmaskInits = bitmaskInits;
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, LevelPresentation::createScene()));
}
