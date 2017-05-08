#include "MenuScene.h"
#include "LevelPresentation.h"
#include "Sound/Audio.h"
#include "Data/DataSetting.h"
#include "Network/GameServer.h"
#include "Jus.h"

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
    initSnow();
    initCamera();
    initMenu();
    initPregameSettingLayer();
    initPregameSwitchTypeGame();
    initNetworkSettingLayer();
    initSelectedLanguage();

    layer->setAttribShaderSensitive(1.f);

    setLanguageLabels(DataSetting::UserData_t::locale);

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
        return true;
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void MenuScene::initMenu()
{
    /* init click Start */
    itemStart = MenuItemLabel::create(createLabel("Classic"), [this](Ref*){ clickClassic(); });

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

void MenuScene::clickClassic()
{
    GameData::mode = GameMode::Classic;
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
    camera->runAction(MoveBy::create(0.3, Vec2(visibleSize.width*3, 0)));
}

void MenuScene::clickExit()
{
    DataSetting::save();
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
        StartServer = !StartServer;
        pregameSwitchTypeGameLayer->setFlagStartServer(StartServer);
    };
}

std::function<void (Locale)> MenuScene::getCallbackClickChangeLanguage() const
{
    return [this] (Locale locale) -> void {
        if (camera->getPositionX() < visibleSize.width) {
            DataSetting::UserData_t::locale = locale;
            setLanguageLabels(DataSetting::UserData_t::locale);
            selectLanguage->setLocale(locale);
        }
    };
}

void MenuScene::setLanguageLabels(Locale locale)
{
    itemStart->setString(Language::get(locale, "Classic"));
    itemSurvival->setString(Language::get(locale, "Survival"));
    itemLocal->setString(Language::get(locale, "Local"));
    itemExit->setString(Language::get(locale, "Exit"));

    if (pregameSettingLayer)
        pregameSettingLayer->setLanguageLabels(locale);

    if (pregameSwitchTypeGameLayer)
        pregameSwitchTypeGameLayer->setLanguageLabels(locale);

    if (gameNavigatorLayer)
        gameNavigatorLayer->setLanguageLabels(locale);
}

void MenuScene::initSelectedLanguage()
{
    selectLanguage = SelectLanguage::create();
    selectLanguage->setLocale(DataSetting::UserData_t::locale);
    selectLanguage->setCallbackClick(getCallbackClickChangeLanguage());

    Size sz = selectLanguage->getContentSize();

    selectLanguage->setPosition(Vec2(visibleSize.width, 0) - Vec2(sz.width * 0.5 + 32, -sz.height * 0.5 - 32));
    selectLanguage->setOpacity(0xff * 0.35);

    addChild(selectLanguage, Levels::LTop);

    selectLanguage->setCameraMask((unsigned short)CameraFlag::USER2);
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

    pregameSwitchTypeGameLayer->setCallbackNext([=](Ref*){
        camera->runAction(MoveBy::create(0.3, Vec2(visibleSize.width, 0)));
        DataSetting::save();
    });

}

void MenuScene::initNetworkSettingLayer()
{
    networkSettingLayer = NetworkSettingLayer::create();
    networkSettingLayer->setCameraMask((unsigned short)CameraFlag::USER2);

    networkSettingLayer->setCallbackStartServer(getCallbackStartServer());

    networkSettingLayer->setCallbackBack([this](Ref*){
        Vec2 point(-visibleSize.width, 0);
        camera->runAction(MoveBy::create(0.2, point));
    });

    networkSettingLayer->setCallbackStart([this](Ref*){
        if (pregameSwitchTypeGameLayer)
            pregameSwitchTypeGameLayer->startLocalGame();

        //Vec2 point(visibleSize.width, 0);
        //camera->runAction(MoveBy::create(0.2, point));
    });

    addChild(networkSettingLayer, LLayer);
}

Label *MenuScene::createLabel(const std::string &text) const
{
    Label *label = Jus::createLabelTTF(text, "fonts/Bicubik.ttf", 32);//Label::createWithTTF(text, "fonts/Bicubik.ttf", 32);
    label->setAdditionalKerning(3);
    label->setCameraMask((unsigned short)CameraFlag::USER2);
    return label;
}

//------------------------------------------------------------------------------------------------------

bool SelectLanguage::init() {

    if (!cocos2d::Sprite::initWithFile("en.png"))
        return false;

    callbackClick = [] (Locale) -> void {};

    counterClicked = DataSetting::UserData_t::locale;

    initListener();

    return true;
}

void SelectLanguage::initListener()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this] (Touch *touch, Event *) -> bool {

        static const std::array<Locale, 3> Locales{Locale::ua, Locale::en, Locale::ru};

        Rect rect = getTextureRect();
        Size size = getContentSize();

        rect.setRect(getPositionX() - size.width / 2, getPositionY() - size.height / 2, size.width, size.height);

        if (rect.containsPoint(touch->getLocation())) {
            Locale locale = Locales[++counterClicked % Locales.size()];
            callbackClick(locale);
        }

        return true;
    };

    listener->onTouchMoved = [this] (Touch *, Event *) -> void {};
    listener->onTouchEnded = [this] (Touch *, Event *) -> void {};


    _director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void SelectLanguage::setCallbackClick(const std::function<void (Locale)> &value)
{
    callbackClick = value;
}

void SelectLanguage::setLocale(Locale locale)
{
    switch (locale) {
    case Locale::ua: setTexture("ua.png"); break;
    case Locale::en: setTexture("en.png"); break;
    case Locale::ru: setTexture("ru.png"); break;
    }
    counterClicked = locale;
}
