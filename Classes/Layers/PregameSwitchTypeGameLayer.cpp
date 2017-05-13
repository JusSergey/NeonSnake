#include "PregameSwitchTypeGameLayer.h"
#include "Scenes/GameView.h"
#include "Constant.h"
#include "Jus.h"

#define offset (Jus::getWidth()*2)

#define LABEL_ITEMS labelLP, labelPP, labelPB, labelP, labelB

USING_NS_CC;

const std::string LOCAL_PLAYER  = "Local Game";
const std::string PLAYER_PLAYER = "Player vs. Player";
const std::string PLAYER_BOT    = "Player vs. Bot";
const std::string ONLY_PLAYER   = "Only Player";
const std::string ONLY_BOT      = "Only Bot";


GameType getGameType (const std::string &strType) {

    if (strType == Language::get(UserData::locale, LOCAL_PLAYER) || strType == LOCAL_PLAYER)
        return LocalGame;

    if (strType == Language::get(UserData::locale, PLAYER_PLAYER))
        return PlayerVSPlayer;

    if (strType == Language::get(UserData::locale, PLAYER_BOT))
        return PlayerVSBot;

    if (strType == Language::get(UserData::locale, ONLY_PLAYER))
        return PlayerAlways;

    if (strType == Language::get(UserData::locale, ONLY_BOT))
        return BotAlways;

//    log("ERROR[%s][%d]", strType.c_str(), (int)UserData::locale);
//    std::terminate();

    return (GameType)-2;
}

Scene* PregameSwitchTypeGameLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'Layer' is an autorelease object

    auto layer = PregameSwitchTypeGameLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool PregameSwitchTypeGameLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if(!Layer::init())
        return false;

    callbackNext = [](Ref*){};

    flagStartServer = InitServer;

    log("initialize PregameSwitchTypeGameLayer...");

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    labelTitle = Jus::createLabelTTF("Select Type Game", "fonts/Bicubik.ttf", 24);

    labelTitle->setAdditionalKerning(3);

    labelTitle->setPosition(offset, visibleSize.height/2 - labelTitle->getContentSize().height);

    addChild(labelTitle, LLayer);

    initSwitchLabels();

    initDrawNode();
    initNavigationMenu();

    setLanguageLabels(UserData::locale);

    if (getItem(UserData::type))
        (getCallbackClickLabel())(getItem(UserData::type));

    return true;
}

void PregameSwitchTypeGameLayer::setFlagStartServer(bool value)
{
    flagStartServer = (value ? 0 : InitServer);
}

void PregameSwitchTypeGameLayer::startLocalGame()
{
    GameView::GoToGameView(GameData::currentLevel, InitAll ^ InitBot ^ InitSecondPlayer ^ flagStartServer);
}

void PregameSwitchTypeGameLayer::setCallbackBackToMenu(const std::function<void(Ref*)> &value)
{
    itemBack->setCallback(value);
}

void PregameSwitchTypeGameLayer::setCallbackNext(const std::function<void (Ref *)> &value)
{
    callbackNext = value;
}

MenuItemLabel *PregameSwitchTypeGameLayer::getItem(GameType type) const
{
    switch (UserData::type) {
    case LocalGame:      return labelLP;
    case PlayerVSPlayer: return labelPP;
    case PlayerVSBot:    return labelPB;
    case PlayerAlways:   return labelP;
    case BotAlways:      return labelB;
    default:             return nullptr;
    }
}

void PregameSwitchTypeGameLayer::initSwitchLabels()
{
    initLabel(labelLP, LOCAL_PLAYER);
    initLabel(labelPP, PLAYER_PLAYER);
    initLabel(labelPB, PLAYER_BOT);
    initLabel(labelP,  ONLY_PLAYER);
    initLabel(labelB,  ONLY_BOT);

    for (auto &i : {LABEL_ITEMS})
        i->setCallback(getCallbackClickLabel());

    log("PregameSwitchTypeGameLayer::initSwitchLabels() : getItem() [%d]", UserData::type);



    widthSolidRect = 0;

    for (const MenuItemLabel* i : {LABEL_ITEMS})
        if (i->getContentSize().width > widthSolidRect)
            widthSolidRect = i->getContentSize().width;

    widthSolidRect += (visibleSize.width - widthSolidRect) / 2;

    Menu *menu = Menu::create(LABEL_ITEMS, nullptr);

    menu->alignItemsVerticallyWithPadding(8);

    menu->setPosition(offset, 0);

    Layer::addChild(menu, LLayer);
}

void PregameSwitchTypeGameLayer::initDrawNode()
{
    DrawNode *node = DrawNode::create();

    float sclFrame = 0.4;

    Vec2 p1(visibleSize.width/2 - widthSolidRect*sclFrame, 0);
    Vec2 p2(visibleSize.width/2 - widthSolidRect*(1.f-sclFrame) + widthSolidRect, visibleSize.height);

    node->setAnchorPoint(Vec2::ZERO);

    node->setPosition(offset-visibleSize.width/2, -visibleSize.height/2);

    addChild(node);

    node->drawSolidRect(p1, p2, Color4F(0, 0, 0, 0.70));
}

void PregameSwitchTypeGameLayer::initLabel(MenuItemLabel *&rvPtr, const std::string &text)
{
    Label *label = Jus::createLabelTTF(text, "fonts/Bicubik.ttf", 36);
    label->setAdditionalKerning(3);
    rvPtr = cocos2d::MenuItemLabel::create(label);
}

void PregameSwitchTypeGameLayer::initNavigationMenu()
{
    Label* lback = Jus::createLabelTTF("<-Back", "fonts/Bicubik.ttf", 36);
    lback->setAdditionalKerning(3);

    Label* lstart = Jus::createLabelTTF("Start", "fonts/Bicubik.ttf", 36);
    lstart->setAdditionalKerning(3);

    itemBack  = MenuItemLabel::create(lback);
    itemStart = MenuItemLabel::create(lstart);

    itemStart->setCallback([this](Ref*) {

        std::string text;

        for (MenuItemLabel *l : {LABEL_ITEMS})
            if (l->getOpacity() == 0xff)
                text = l->getString();

        if (text == Language::get(DataSetting::UserData_t::locale, PLAYER_PLAYER))
            GameView::GoToGameView(GameData::currentLevel, InitAll ^ InitLocalPlayer ^ InitBot ^ InitServer);

        else if (text == Language::get(DataSetting::UserData_t::locale, LOCAL_PLAYER))
            callbackNext(nullptr);

        else if (text == Language::get(DataSetting::UserData_t::locale, PLAYER_BOT))
            GameView::GoToGameView(GameData::currentLevel, InitAll ^ InitLocalPlayer ^ InitSecondPlayer ^ InitServer);

        else if (text == Language::get(DataSetting::UserData_t::locale, ONLY_PLAYER))
            GameView::GoToGameView(GameData::currentLevel, InitAll ^ InitLocalPlayer ^ InitBot ^ InitSecondPlayer ^ InitServer);

        else if (text == Language::get(DataSetting::UserData_t::locale, ONLY_BOT))
            GameView::GoToGameView(GameData::currentLevel, InitAll ^ InitLocalPlayer ^ InitFirstPlayer ^ InitSecondPlayer ^ InitServer);

    });

    itemBack ->setPosition(Vec2(offset, 0) - visibleSize + itemBack->getContentSize()*0.65);
    itemStart->setPosition(Vec2(offset, -visibleSize.height) + Vec2(-itemStart->getContentSize().width*0.65, itemStart->getContentSize().height*0.65));

    itemStart->setString(UserData::type == LocalGame ? "Next->" : "Start");

    addChild(Menu::create(itemBack, itemStart, nullptr), LLayer);
}

void PregameSwitchTypeGameLayer::setLanguageLabels(Locale locale)
{
    labelB->setString(Language::get(locale, "Only Bot"));
    labelLP->setString(Language::get(locale, "Local Game"));
    labelP->setString(Language::get(locale, "Only Player"));
    labelPB->setString(Language::get(locale, "Player vs. Bot"));
    labelPP->setString(Language::get(locale, "Player vs. Player"));
    labelTitle->setString(Language::get(locale, "Select Type Game"));

    itemBack->setString(std::string("<- ") + Language::get(locale, "Back"));

    if (UserData::type == LocalGame)
        itemStart->setString(Language::get(locale, "Next") + " ->");

    else itemStart->setString(Language::get(locale, "Start"));

    itemBack ->setPosition(Vec2(offset, 0) - visibleSize + itemBack->getContentSize()*0.65);
    itemStart->setPosition(Vec2(offset, -visibleSize.height) + Vec2(-itemStart->getContentSize().width*0.65, itemStart->getContentSize().height*0.65));
}

std::function<void (Ref *)> PregameSwitchTypeGameLayer::getCallbackClickLabel()
{
    return [this](Ref *node){

        for (MenuItemLabel *label : {LABEL_ITEMS}) {

            if (node == label) {

                label->setOpacity(0xff);

                UserData::type = getGameType(label->getString());
                DataSetting::save();

                if (itemStart)
                    itemStart->setString(UserData::type == LocalGame ? Language::get(DataSetting::UserData_t::locale, "Next") + " ->" :
                                                                       Language::get(DataSetting::UserData_t::locale, "Start"));

            } else label->setOpacity(0xff * 0.25);

        }
    };
}
