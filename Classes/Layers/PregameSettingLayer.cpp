#include "PregameSettingLayer.h"
#include "Data/DataSetting.h"
#include "Jus.h"

static const float BORDER_1 = 15;
static const float BORDER_2 = BORDER_1 + 15;
static const float BORDER_3 = BORDER_2 + 15;
static const float scl = 0.90;

std::map<SwitchLevelGame::_Tag, SwitchLevelGame *> SwitchLevelGame::globalObjects;

#define offsetX (Jus::getWidth())

enum {
    LDNode,
    LOther
};

USING_NS_CC;

float getP2(float p) {
    float r = BORDER_2 + (Jus::getHeight() / 2 - BORDER_1 / 2 - BORDER_2) * p;
    return r;
}

float getP1(float p) {
    float r = Jus::getHeight() / 2 + BORDER_1 / 2 + (Jus::getHeight() / 2 - BORDER_1 / 2 - BORDER_2) * p;
    return r;
}

// on "init" you need to initialize your instance
bool PregameSettingLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if(!Layer::init())
        return false;

    log("initialize PregameSettingLayer...");

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    initItemNext();
    initItemBackToMenu();
    initMenu();

    auto positioningY1 = [](float p) -> float { return getP1(p); };
    auto positioningY2 = [](float p) -> float { return getP2(p); };

    initLabelSetting(labelPlayerTitle, "Player", 0.85, positioningY1);
    initLabelSetting(labelOpponentTitle, "Bot",    0.85, positioningY2);

    initLabelSetting(labelPlayerColor,  "Color Player", 0.40, positioningY1);
    initLabelSetting(labelOpponentColor, "Color Bot",   0.40, positioningY2);

    initTextFields(positioningY1, fieldPlayer, UserData::playerName);
    initTextFields(positioningY2, fieldOpponent, UserData::opponentName);

    initColors(swColorsPlayer1, positioningY1, PlayerTag);
    initColors(swColorsPlayer2, positioningY2, OpponentTag);

    initDrawNode();
    initScrollViewLevels();

    initBlackBackground();

    return true;
}

void PregameSettingLayer::addChild(Node *child, int level)
{
    child->setPositionX(child->getPositionX() + offsetX);
    Layer::addChild(child, level);
}

void PregameSettingLayer::setCallbackNext(const std::function<void(Ref *)> &value)
{
    itemNext->setCallback([=](Ref* ref){
        UserData::playerName = fieldPlayer->getString();
        UserData::opponentName = fieldOpponent->getString();
        DataSetting::save();
        value(ref);
    });
}

void PregameSettingLayer::setCallbackBackToMenu(const std::function<void(Ref *)> &value)
{
    itemBackToMenu->setCallback([=](Ref* ref){
//        blackBackground->runAction(FadeOut::create(0.3));
        UserData::playerName = fieldPlayer->getString();
        UserData::opponentName = fieldOpponent->getString();
        DataSetting::save();
        value(ref);
    });
}

void PregameSettingLayer::runToThisMenu()
{
    log("runToThisMenu");
    blackBackground->runAction(FadeTo::create(0.3, 0xff * 0.9));
}

void PregameSettingLayer::setSwitchColor(PregameSettingLayer::SwitchColorContainer_t &sw, const Color3B &color)
{
    for (auto i : sw) {
        if (i.first->getColor() == color) {
            i.first->setScale(1.3);
            i.first->setOpacity(0xff);
            i.second = true;
        } else {
            i.first->setScale(0.9);
            i.first->setOpacity(0xff * 0.30);
            i.second = false;
        }
    }
}

std::function<void (const Color3B &color, int tag)> PregameSettingLayer::getCallbackSelectColor()
{
    return [this] (const Color3B &color, int tag) -> void {

        switch (tag) {
            case PlayerTag : {
                setSwitchColor(swColorsPlayer1, color);
                UserData::playerColor = color;
            } break;
            case OpponentTag : {
                setSwitchColor(swColorsPlayer2, color);
                UserData::opponentColor = color;
            } break;
        }

        DataSetting::save();

    };
}

void PregameSettingLayer::initItemNext()
{
    itemNext = MenuItemLabel::create(Jus::createLabelTTF("Next ->", "fonts/Bicubik.ttf", 32));

    itemNext->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);

    itemNext->setPosition(visibleSize.width - BORDER_3, BORDER_3);
}

void PregameSettingLayer::initItemBackToMenu()
{
    itemBackToMenu = MenuItemLabel::create(Jus::createLabelTTF("<- Back", "fonts/Bicubik.ttf", 32));

    itemBackToMenu->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

    itemBackToMenu->setPosition(visibleSize.width / 2 + BORDER_3, BORDER_3);
}

void PregameSettingLayer::initMenu()
{
    auto menu = Menu::create(itemNext, itemBackToMenu, nullptr);

    menu->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    menu->setPosition(Vec2::ZERO);

    addChild(menu, LOther);
}

void PregameSettingLayer::initDrawNode()
{

    auto callInitBack = [this] (const Vec2 &pos, const std::string &path, Sprite* &rvPtrSprite) {

        const float scaleX = 1.0 - ((resolutionDisplay.width / resolutionDisplay.height) -
                                    (visibleSize.width / visibleSize.height)) / 2;

        rvPtrSprite = Sprite::create(path);
        rvPtrSprite->setPosition(pos);
        rvPtrSprite->setOpacity(0xff * 0.15);

        rvPtrSprite->setScaleX(scaleX);

        addChild(rvPtrSprite, LDNode);
    };

    callInitBack(Jus::getDisplayPoint(0.26, 0.740), "PregameMenu.png", playerBackground);
    callInitBack(Jus::getDisplayPoint(0.26, 0.270), "PregameMenu.png", opponentBackground);
    callInitBack(Jus::getDisplayPoint(0.75, 0.501), "SelectLevelMenu.png", scrollBackground);

    log("init draw");
    dnode = DrawNode::create();

    dnode->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    dnode->setPosition(Vec2::ZERO);

    addChild(dnode, LDNode);

}

void PregameSettingLayer::initBlackBackground()
{
    blackBackground = Sprite::create("BlackBackground.png");
    blackBackground->setPosition(Jus::getCenter());
    blackBackground->setOpacity(0);

    addChild(blackBackground, LDNode-1);
}

void PregameSettingLayer::initLabelSetting(Label *&rvLabel, const std::string &title, float procentY, const std::function<float (float)> &positioning)
{
    rvLabel = Jus::createLabelTTF(title, "fonts/Bicubik.ttf", 26);
    rvLabel->setAdditionalKerning(3);

    rvLabel->setPosition(visibleSize.width / 4, positioning(procentY));

    addChild(rvLabel, LOther);
}

void PregameSettingLayer::initColors(SwitchColorContainer_t &sw, const std::function<float (float)> &positioning, int tag)
{
    auto colors = {Color3B::MAGENTA, Color3B::BLUE, Color3B::GREEN, Color3B::YELLOW, Color3B::ORANGE, Color3B::RED};

    float width = (visibleSize.width / 2 - BORDER_2 - BORDER_1 / 2) / (colors.size()+1);

    int index = 0;

    for (const auto &i : colors) {

        auto switchColor = SwitchColorSnake::create();

        switchColor->setCallbackSelectColor(getCallbackSelectColor());
        switchColor->setTag(tag);
        switchColor->setColor(i);
        switchColor->setPosition(BORDER_2 + width * ++index, positioning(0.20));

        addChild(switchColor, LOther);

        sw.push_back(SwitchColorObj_t(switchColor, switchColor->getColor() == UserData::playerColor));
    }

    switch (tag) {
        case PlayerTag: {
            setSwitchColor(sw, UserData::playerColor);
        } break;
        case OpponentTag: {
            setSwitchColor(sw, UserData::opponentColor);
        } break;
    }

}

void PregameSettingLayer::initTextFields(const std::function<float (float)> &positioning, ui::TextField *&field, const std::string &name)
{
    field = ui::TextField::create("Enter name", "fonts/Bicubic.ttf", (float)24 / _director->getContentScaleFactor());
    field->setPlaceHolderColor(Color3B::GRAY);
    field->setMaxLength(20);
    field->setMaxLengthEnabled(true);
    field->setString(name);
    field->setPosition({visibleSize.width / 4, positioning(0.70)});

    addChild(field, LOther);
}

void PregameSettingLayer::initScrollViewLevels()
{
    auto scroll = ui::ScrollView::create();

    const float HVScale = visibleSize.height / resolutionDisplay.height;

    scroll->setCameraMask((unsigned short)CameraFlag::USER2);
    scroll->setPosition(Vec2::ZERO + Vec2(0, (itemNext->getPositionY() +
                                              itemNext->getContentSize().height * itemNext->getAnchorPoint().y) * HVScale + BORDER_1));
    scroll->setDirection(ui::ScrollView::Direction::VERTICAL);
    scroll->setBounceEnabled(true);
    scroll->setContentSize(Size(visibleSize.width/2 - BORDER_2, scrollBackground->getContentSize().height - scroll->getPositionY() - BORDER_1));

    scrollBackground->addChild(scroll, LOther);



    for (int i = 1; i <= countLevels; i++) {

        auto level = SwitchLevelGame::create();

        levels.push_back(level);
        level->setTag(i);
        level->setPosition({ scrollBackground->getContentSize().width / 2,
                             level->getContentSize().height * ((float)countLevels - i + 0.5) + BORDER_3});

        scroll->addChild(level, 1);
    }

    const float h = levels[0]->getContentSize().height;
    scroll->setInnerContainerSize(Size(visibleSize.width / 2.5, h * countLevels + BORDER_3));

}

void PregameSettingLayer::setLanguageLabels(Locale locale)
{
    itemBackToMenu->setString(std::string("<- ") + Language::get(locale, "Back"));
    itemNext->setString(Language::get(locale, "Next") + " ->");

    labelPlayerTitle->setString(Language::get(locale, "Player name"));
    labelOpponentTitle->setString(Language::get(locale, "Opponent name"));
    labelPlayerColor->setString(Language::get(locale, "Color Player"));
    labelOpponentColor->setString(Language::get(locale, "Color Opponent"));
}

bool SwitchColorSnake::init()
{
    if (!Sprite::init())
        return false;

    callbackSelectColor = [](const Color3B &, int){};

    setTexture("eat.png");

    auto listener = EventListenerTouchOneByOne::create();

    listener->onTouchBegan = [=](Touch *touch, Event *) -> bool {
        if (getBoundingBox().containsPoint(touch->getLocation() + Vec2(offsetX, 0)))
            callbackSelectColor(Sprite::getColor(), Sprite::getTag());

        return true;
    };

    listener->onTouchEnded = [](Touch*, Event*){};

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void SwitchColorSnake::setCallbackSelectColor(const std::function<void (const Color3B &, int)> &callback)
{
    callbackSelectColor = callback;
}

bool SwitchLevelGame::init()
{
    static int level = 1;

    if (level > countLevels)
        level = 1;

    myTag = level;
    SwitchLevelGame::globalObjects.erase(myTag);
    SwitchLevelGame::globalObjects.insert(std::make_pair(myTag, this));

    std::string source = std::string("MiniLevels/level_") + StringUtils::toString(level++) + ".png";

    if (!ui::Button::init(source))
        return false;

    setSelectLevel(GameData::currentLevel);

    return true;
}


void SwitchLevelGame::onTouchEnded(Touch *touch, Event *unusedEvent)
{
    ui::Button::onTouchEnded(touch, unusedEvent);

    if (touch->getStartLocation().distance(touch->getLocation()) < Jus::getWidth() * 0.01 /* < 1% of width */)
        GameData::currentLevel = getTag();

    SwitchLevelGame::setSelectLevel(GameData::currentLevel);

}

void SwitchLevelGame::setSelectLevel(int selectLevel)
{
    for (auto &objPair : SwitchLevelGame::globalObjects) {
        int lvl = objPair.first;
        objPair.second->setOpacity(lvl == selectLevel ? 0xff : 0xff*0.75);
        objPair.second->setScale(lvl == selectLevel ? scl + 0.05 : scl);
    }
}
