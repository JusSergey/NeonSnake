#include "PregameSwitchTypeGameLayer.h"
#include "Scenes/GameView.h"
#include "Constant.h"
#include "Jus.h"

#define offset (Jus::getWidth()*2)

#define LABEL_ITEMS labelLP, labelPP, labelPB, labelP, labelB

USING_NS_CC;

const std::string LOCAL_PLAYER  = "Local Player";
const std::string PLAYER_PLAYER = "Player vs. Player";
const std::string PLAYER_BOT    = "Player vs. Bot";
const std::string ONLY_PLAYER   = "Only Player";
const std::string ONLY_BOT      = "Only Bot";


GameType getGameType (const std::string &strType) {

    if (strType == LOCAL_PLAYER)
        return LocalGame;

    if (strType == PLAYER_PLAYER)
        return PlayerVSPlayer;

    if (strType == PLAYER_BOT)
        return PlayerVSBot;

    if (strType == ONLY_PLAYER)
        return PlayerAlways;

    if (strType == ONLY_BOT)
        return BotAlways;

    return (GameType)-1;
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

    flagStartServer = InitServer;

    log("initialize PregameSwitchTypeGameLayer...");

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    Label* label = Label::createWithSystemFont("Select Type Game", "fonts/Bicubik.ttd", 36);

    label->setAdditionalKerning(3);

    label->setPosition(offset, visibleSize.height/2 - label->getContentSize().height);

    addChild(label, LLayer);

    initSwitchLabels();
    initDrawNode();
    initNavigationMenu();

    log("good");

    return true;
}

void PregameSwitchTypeGameLayer::setFlagStartServer(bool value)
{
    log("sserv: %s", value ? "true" : "false");
    flagStartServer = (value ? 0 : InitServer);
}

void PregameSwitchTypeGameLayer::setCallbackBackToMenu(const std::function<void(Ref*)> &value)
{
    itemBack->setCallback(value);
}

MenuItemLabel *PregameSwitchTypeGameLayer::getItem(GameType type) const
{
    switch (UserData::type) {
    case LocalGame:    return labelLP;
    case PlayerVSPlayer: return labelPP;
    case PlayerVSBot:    return labelPB;
    case PlayerAlways:   return labelP;
    case BotAlways:      return labelB;
    default:             return nullptr;
    }
}

//void PregameSwitchTypeGameLayer::addChild(Node *child, int localZOrder)
//{
//    child->setPositionX(child->getPositionX() + offset);
//    Layer::addChild(child, localZOrder);
//}

void PregameSwitchTypeGameLayer::initSwitchLabels()
{
    initLabel(labelLP, LOCAL_PLAYER);
    initLabel(labelPP, PLAYER_PLAYER);
    initLabel(labelPB, PLAYER_BOT);
    initLabel(labelP,  ONLY_PLAYER);
    initLabel(labelB,  ONLY_BOT);

    for (auto &i : {LABEL_ITEMS})
        i->setCallback(getCallbackClickLabel());

    if(getItem(UserData::type))
        (getCallbackClickLabel())(getItem(UserData::type));

    widthSolidRect = 0;

    for (const MenuItemLabel* i : {LABEL_ITEMS})
        if (i->getContentSize().width > widthSolidRect)
            widthSolidRect = i->getContentSize().width;

    widthSolidRect += (visibleSize.width - widthSolidRect) / 2;
    log("width %f ", widthSolidRect);

    Menu *menu = Menu::create(LABEL_ITEMS, nullptr);

    menu->alignItemsVerticallyWithPadding(8);

    menu->setPosition(offset, 0);

    Layer::addChild(menu, LLayer);
    log("INITS");
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
    rvPtr = cocos2d::MenuItemLabel::create(Label::createWithSystemFont(text, "monospace", 48));
}

void PregameSwitchTypeGameLayer::initNavigationMenu()
{
    Label* lback = Label::createWithTTF("<-Back", "fonts/Bicubik.ttf", 36);
    lback->setAdditionalKerning(3);

    Label* lstart = Label::createWithTTF("Start", "fonts/Bicubik.ttf", 36);
    lstart->setAdditionalKerning(3);

    itemBack  = MenuItemLabel::create(lback);
    itemStart = MenuItemLabel::create(lstart);

    itemStart->setCallback([this](Ref*) {

        std::string text;

        for (MenuItemLabel *l : {LABEL_ITEMS})
            if (l->getOpacity() == 0xff)
                text = l->getString();

        if (text == PLAYER_PLAYER)
            GameView::GoToGameView(GameData::currentLevel, InitAll ^ InitLocalPlayer ^ InitBot ^ InitServer);

        else if (text == LOCAL_PLAYER){
            GameView::GoToGameView(GameData::currentLevel, InitAll ^ InitBot ^ InitSecondPlayer ^ flagStartServer);
        }

        else if (text == PLAYER_BOT)
            GameView::GoToGameView(GameData::currentLevel, InitAll ^ InitLocalPlayer ^ InitSecondPlayer ^ InitServer);

        else if (text == ONLY_PLAYER)
            GameView::GoToGameView(GameData::currentLevel, InitAll ^ InitLocalPlayer ^ InitBot ^ InitSecondPlayer ^ InitServer);

        else if (text == ONLY_BOT)
            GameView::GoToGameView(GameData::currentLevel, InitAll ^ InitLocalPlayer ^ InitFirstPlayer ^ InitSecondPlayer ^ InitServer);

    });

    itemBack ->setPosition(Vec2(offset, 0) - visibleSize + itemBack->getContentSize()*0.65);
    itemStart->setPosition(Vec2(offset, -visibleSize.height) + Vec2(-itemStart->getContentSize().width*0.65, itemStart->getContentSize().height*0.65));

    addChild(Menu::create(itemBack, itemStart, nullptr), LLayer);
}

std::function<void (Ref *)> PregameSwitchTypeGameLayer::getCallbackClickLabel()
{
    return [this](Ref *node){

        for (MenuItemLabel *l : {LABEL_ITEMS}) {

            if (node == l) {

                l->setOpacity(0xff);
                UserData::type = getGameType(l->getString());
                DataSetting::save();

            } else {
                l->setOpacity(0xff * 0.25);
            }

        }

        log("clicked item");
    };
}
