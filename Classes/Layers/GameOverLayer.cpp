#include "GameOverLayer.h"
#include "Jus.h"

USING_NS_CC;

static const std::string FONT_NAME    = "fonts/Bicubik.ttf";
static const std::string PREFIX_WIN   = "win: ";
static const std::string PREFIX_SCORE = "score: ";
static const std::string PREFIX_BONUS = "bonus: ";
static const std::string PREFIX_EAT   = "eat: ";

static const int SZ_FONT_WIN = 32;
static const int SZ_FONT_DEF = 24;

// on "init" you need to initialize your instance
bool GameOverLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if(!Sprite::initWithFile("PregameMenu.png"))
        return false;

    setOpacity(0xff * 0.35);

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    initLabels();

    return true;
}

void GameOverLayer::initLabels()
{
    labelWin = Jus::createLabelTTF(PREFIX_WIN, FONT_NAME, SZ_FONT_WIN);
    labelWin->setPosition(Jus::getPointNode(this, Vec2(0.5, 1)) - Vec2(0, labelWin->getContentSize().height / 2));
    addChild(labelWin);

    labelScore = Jus::createLabelTTF(PREFIX_SCORE, FONT_NAME, SZ_FONT_DEF);
    labelScore->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    labelScore->setPosition(SZ_FONT_DEF, Jus::getPointNode(this, Vec2(0, 0.7)).y);
    addChild(labelScore);

    labelBonus = Jus::createLabelTTF(PREFIX_BONUS, FONT_NAME, SZ_FONT_DEF);
    labelBonus->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    labelBonus->setPosition(SZ_FONT_DEF, Jus::getPointNode(this, Vec2(0, 0.5)).y);
    addChild(labelBonus);

    labelEat = Jus::createLabelTTF(PREFIX_EAT, FONT_NAME, SZ_FONT_DEF);
    labelEat->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    labelEat->setPosition(SZ_FONT_DEF, Jus::getPointNode(this, Vec2(0, 0.3)).y);
    addChild(labelEat);
}

void GameOverLayer::setNameWin(const std::string &name)
{
    if (labelWin) {
        labelWin->setString(PREFIX_WIN + name);
    }
}

void GameOverLayer::setScore(int count)
{
    if (labelScore) {
        _score = count;
        labelScore->setString(PREFIX_SCORE + StringUtils::toString(_score));
    }
}

void GameOverLayer::setBonus(int count)
{
    if (labelBonus) {
        _bonus = count;
        labelBonus->setString(PREFIX_BONUS + StringUtils::toString(_bonus));
    }
}

void GameOverLayer::setEat(int count)
{
    if (labelEat) {
        _eat = count;
        labelEat->setString(PREFIX_EAT + StringUtils::toString(_eat));
    }
}
