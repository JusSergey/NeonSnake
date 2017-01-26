#include "PregameSettingLayer.h"
#include "Data/DataSetting.h"
#include "Jus.h"

static const float border_1 = 15;
static const float border_2 = border_1 + 15;
static const float border_3 = border_2 + 15;

#define offsetX (Jus::getWidth())

enum {
    LDNode,
    LOther
};

USING_NS_CC;

float getP2(float p) {
    float r = border_2 + (Jus::getHeight() / 2 - border_1 / 2 - border_2) * p;
    return r;
}

float getP1(float p) {
    float r = Jus::getHeight() / 2 + border_1 / 2 + (Jus::getHeight() / 2 - border_1 / 2 - border_2) * p;
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

    initLabelSetting("Player", 0.85, positioningY1);
    initLabelSetting("Bot",    0.85, positioningY2);

    initLabelSetting("Color Player", 0.40, positioningY1);
    initLabelSetting("Color Bot",    0.40, positioningY2);

    initTextFields(positioningY1, fieldPlayer, UserData::playerName);
    initTextFields(positioningY2, fieldOpponent, UserData::opponentName);

    initColors(swColorsPlayer1, positioningY1, PlayerTag);
    initColors(swColorsPlayer2, positioningY2, OpponentTag);

    initScrollViewLevels();

    initDrawNode();

    return true;
}

void PregameSettingLayer::addChild(Node *child, int level)
{
    child->setPositionX(child->getPositionX() + offsetX);
    Layer::addChild(child, level);
}

void PregameSettingLayer::setCallbackNext(const std::function<void(Ref *)> &value)
{
    itemStart->setCallback([=](Ref* ref){
        UserData::playerName = fieldPlayer->getString();
        UserData::opponentName = fieldOpponent->getString();
        DataSetting::save();
        value(ref);
    });
}

void PregameSettingLayer::setCallbackBackToMenu(const std::function<void(Ref *)> &value)
{
    itemBackToMenu->setCallback([=](Ref* ref){
        UserData::playerName = fieldPlayer->getString();
        UserData::opponentName = fieldOpponent->getString();
        DataSetting::save();
        value(ref);
    });
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
    itemStart = MenuItemLabel::create(Label::createWithTTF("Next ->", "fonts/Bicubik.ttf", 32));

    itemStart->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);

    itemStart->setPosition(visibleSize.width - border_3, border_3);
}

void PregameSettingLayer::initItemBackToMenu()
{
    itemBackToMenu = MenuItemLabel::create(Label::createWithTTF("<- Back", "fonts/Bicubik.ttf", 32));

    itemBackToMenu->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

    itemBackToMenu->setPosition(visibleSize.width / 2 + border_3, border_3);
}

void PregameSettingLayer::initMenu()
{
    auto menu = Menu::create(itemStart, itemBackToMenu, nullptr);

    menu->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    menu->setPosition(Vec2::ZERO);

    addChild(menu, LOther);
}

void PregameSettingLayer::initDrawNode()
{
    log("init draw");
    dnode = DrawNode::create();

    dnode->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    dnode->setPosition(Vec2::ZERO);

    addChild(dnode, LDNode);

    // draw main rect
    dnode->drawSolidRect(Vec2(border_1, border_1),
                         Jus::getDisplayPoint(1, 1) - Vec2(border_1, border_1),
                         Color4F(0, 0.05, 0.05, 0.6));

    // draw player layer
    dnode->drawSolidRect(Vec2(border_2, visibleSize.height / 2 + border_1),
                         Vec2(visibleSize.width / 2 - border_1 / 2, visibleSize.height - border_2),
                         Color4F(0, 0, 0, 0.7));

    // draw bot layer
    dnode->drawSolidRect(Vec2(border_2, border_2),
                         Vec2(visibleSize.width / 2 - border_1 / 2, visibleSize.height / 2 - border_1 / 2),
                         Color4F(0, 0, 0, 0.8));

    // levels seletced layer
    dnode->drawSolidRect(Vec2(visibleSize.width / 2 + border_2 / 2, border_2),
                         Vec2(visibleSize.width - border_2, visibleSize.height - border_2),
                         Color4F(0, 0, 0, 0.8));

}

void PregameSettingLayer::initLabelSetting(const std::string &title, float procentY, const std::function<float (float)> &positioning)
{
    Label *labelTitle = Label::createWithTTF(title, "fonts/Bicubik.ttf", 32);
    labelTitle->setAdditionalKerning(3);

    labelTitle->setPosition(visibleSize.width / 4, positioning(procentY));

    addChild(labelTitle, LOther);
}

void PregameSettingLayer::initColors(SwitchColorContainer_t &sw, const std::function<float (float)> &positioning, int tag)
{
    auto colors = {Color3B::MAGENTA, Color3B::BLUE, Color3B::GREEN, Color3B::YELLOW, Color3B::ORANGE, Color3B::RED};

    float width = (visibleSize.width / 2 - border_2 - border_1 / 2) / (colors.size()+1);

    int index = 0;

    for (const auto &i : colors) {

        auto switchColor = SwitchColorSnake::create();

        switchColor->setCallbackSelectColor(getCallbackSelectColor());

        switchColor->setTag(tag);

        switchColor->setColor(i);

        switchColor->setPosition(border_2 + width * ++index, positioning(0.20));

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
    field = ui::TextField::create("Enter name", "fonts/Bicubic.ttf", 24);
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

    scroll->setCameraMask((unsigned short)CameraFlag::USER2);

    scroll->setPosition({visibleSize.width * 0.50f, border_3*2});

    scroll->setDirection(ui::ScrollView::Direction::VERTICAL);

    scroll->setBounceEnabled(true);

    addChild(scroll, LOther);

    float scl = 0.35;

    for (int i = 1; i <= countLevels; i++) {

        auto level = SwitchLevelGame::create();

        level->setTag(i);

        level->setScale(scl);
        level->setPosition({visibleSize.width * 0.25f, border_1*4 + (level->getBoundingBox().size.height + border_3) * (countLevels - i + 1) - level->getBoundingBox().size.height / 2});

        scroll->addChild(level, 1);
    }

    scroll->setContentSize(Size(visibleSize.width/2 - border_1 - border_2, visibleSize.height - border_3*3));

    float h = visibleSize.height * scl + border_3;

    scroll->setInnerContainerSize(Size(visibleSize.width / 2.5, h*countLevels + border_3*2));

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

    std::string source = std::string("Levels/level_") + StringUtils::toString(level++) + ".png";

    if (!ui::Button::init(source))
        return false;

    return true;
}


void SwitchLevelGame::onTouchEnded(Touch *touch, Event *unusedEvent)
{
    ui::Button::onTouchEnded(touch, unusedEvent);

    if (touch->getStartLocation().distance(touch->getLocation()) < Jus::getWidth() * 0.01 /* < 1% of width */)
        GameData::currentLevel = getTag();

}
