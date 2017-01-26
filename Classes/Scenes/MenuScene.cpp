#include "MenuScene.h"
#include "LevelPresentation.h"
#include "Sound/Audio.h"
#include "Data/DataSetting.h"
#include "Network/GameServer.h"

USING_NS_CC;

Scene* MenuScene::createScene(int bitmask, int bitmaskGN)
{
    // 'scene' is an autorelease object
    auto scene = cocos2d::Scene::createWithPhysics();

    auto layer = MenuScene::create(bitmask, bitmaskGN);
    layer->world = scene->getPhysicsWorld();

    layer->world->setGravity(cocos2d::Vec2::ZERO);

    // add layer as a child to scene
    scene->addChild(layer);

    layer->retain();
    scene->retain();

    // return the scene
    return scene;
}

MenuScene *MenuScene::create(int bitmask, int bitmaskGN)
{
    MenuScene *pRet = new(std::nothrow) MenuScene();
    if (pRet) {
        pRet->bitmaskInitsGameLayer = bitmask;
        pRet->bitmaskInitsGameNavigator = bitmaskGN;
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

void MenuScene::GoToMenuScene(int bitmask, int bitmaskGN)
{
    Director::getInstance()->replaceScene(TransitionFade::create(1, MenuScene::createScene(bitmask, bitmaskGN)));
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
    levelIndex = 1;
    if(!GameView::init())
        return false;

    if (gameNavigatorLayer)
        gameNavigatorLayer->setCameraMask((unsigned short)CameraFlag::USER2);

    log("initialize MenuScene...");

    initDrawNode();
    initTouches();
//    initSnow();
    initCamera();
    initMenu();
    initPregameSettingLayer();
    initPregameSwitchTypeGame();
    initNetworkSettingLayer();

    layer->setAttribShaderSensitive(1.f);

    if (UserData::playingBackgroundMusic)
        Audio::getInstance()->playBackgroundMusicMenu();

    return true;
}


void MenuScene::initDrawNode()
{
    draw = DrawNode::create();
    draw->setPosition(Vec2::ZERO);
    Vec2 p1(visibleSize.width * 0.3, 0);
    Vec2 p2(visibleSize.width * 0.7, visibleSize.height);

    addChild(draw, LMenu);

    draw->setCameraMask((unsigned short)CameraFlag::USER2);

    draw->drawSolidRect(p1, p2, Color4F(0, 0, 0, 0.50));
}

void MenuScene::initTouches()
{
    auto listener = EventListenerTouchOneByOne::create();

    listener->onTouchBegan = [this](Touch *touch, Event*) -> bool {
        setPositionEat(touch->getLocation());
//        updateBot();
        return true;
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void MenuScene::initMenu()
{
    /* init click Start */
    itemStart = MenuItemLabel::create(createLabel("Start"), [this](Ref*){ clickStart(); });

    /* init click Survival */
    itemSurvival = MenuItemLabel::create(createLabel("Survival"), [this](Ref*){ clickSurvival(); });

    /* init click Local */
    itemLocal = MenuItemLabel::create(createLabel("Local"), [this](Ref*) { clickLocal(); });

    /* init click Exit */
    itemExit = MenuItemLabel::create(createLabel("Exit"), [this](Ref*){ clickExit(); });

    menu = Menu::create(itemStart, itemSurvival, itemLocal, itemExit, nullptr);
    menu->setCameraMask((unsigned short)CameraFlag::USER2);
    menu->alignItemsVerticallyWithPadding(10);
    menu->setPosition(visibleSize / 2);
    addChild(menu, LMenu);
}

void MenuScene::clickStart()
{
    GameData::mode = GameMode::Default;
    camera->runAction(MoveBy::create(0.3, Vec2(visibleSize.width, 0)));
}

void MenuScene::clickSurvival()
{
    GameData::mode = GameMode::Survival;
    camera->runAction(MoveBy::create(0.3, Vec2(visibleSize.width, 0)));
}

void MenuScene::clickLocal()
{
    GameData::mode = GameMode::Local;
    camera->runAction(MoveBy::create(0.3, Vec2(0, visibleSize.height)));
}

void MenuScene::clickExit()
{
    Director::getInstance()->end();
}

std::function<void ()> MenuScene::getCallbackMusicButton() const
{
    return [this] () -> void {
        if (UserData::playingBackgroundMusic)
            Audio::getInstance()->playBackgroundMusicMenu();

        else Audio::getInstance()->pauseBackgroundMusic();
    };
}

std::function<void (Ref *)> MenuScene::getCallbackStartServer() const
{
    return [this](Ref*){
        pregameSwitchTypeGameLayer->setFlagStartServer(true);
        isClickStartServer = true;
    };
}

void MenuScene::initCamera()
{
    camera = Camera::createOrthographic(visibleSize.width, visibleSize.height, 1, 1024);
    camera->setPositionZ(1);
    camera->setContentSize(visibleSize);
    camera->setAnchorPoint(Vec2(0.5, 0.5));
    camera->setPosition(visibleSize / 2);
    camera->setCameraFlag(CameraFlag::USER2);
    addChild(camera, LTop);
}

void MenuScene::initSnow()
{
    auto snow = ParticleRain::create();
    snow->setAtlasIndex(540);
    snow->setStartSize(7);
    addChild(snow);
}

void MenuScene::initPregameSettingLayer()
{
    addChild(pregameSettingLayer = PregameSettingLayer::create());

    pregameSettingLayer->setCameraMask((unsigned short)CameraFlag::USER2);

    pregameSettingLayer->setCallbackBackToMenu([=](Ref*){
        camera->runAction(MoveBy::create(0.3, Vec2(-visibleSize.width, 0)));
        DataSetting::save();
    });

    pregameSettingLayer->setCallbackNext([this](Ref*){
        camera->runAction(MoveBy::create(0.3, Vec2(visibleSize.width, 0)));
        DataSetting::save();
    });
}

void MenuScene::initPregameSwitchTypeGame()
{
    pregameSwitchTypeGameLayer = PregameSwitchTypeGameLayer::create();

    pregameSwitchTypeGameLayer->setCameraMask((unsigned short)CameraFlag::USER2);

    pregameSwitchTypeGameLayer->setPosition(visibleSize/2);

    addChild(pregameSwitchTypeGameLayer, LLayer);

    pregameSwitchTypeGameLayer->setCallbackBackToMenu([=](Ref*){
        camera->runAction(MoveBy::create(0.3, Vec2(-visibleSize.width, 0)));
        DataSetting::save();
    });

}

void MenuScene::initNetworkSettingLayer()
{
    networkSettingLayer = NetworkSettingLayer::create();
    networkSettingLayer->setCameraMask((unsigned short)CameraFlag::USER2);

    networkSettingLayer->setCallbackStartServer(getCallbackStartServer());

    networkSettingLayer->setCallbackBack([this](Ref*){
        Vec2 point(0, -visibleSize.height);
        camera->runAction(MoveBy::create(0.2, point));
    });

    networkSettingLayer->setCallbackNext([this](Ref*){
        Vec2 point(visibleSize.width, -visibleSize.height);
        camera->runAction(MoveBy::create(0.2, point));
    });

    addChild(networkSettingLayer, LLayer);
}

Label *MenuScene::createLabel(const std::string &text) const
{
    Label *label = Label::createWithTTF(text, "fonts/Bicubik.ttf", 32);
    label->setAdditionalKerning(3);
    label->setCameraMask((unsigned short)CameraFlag::USER2);
    return label;
}


