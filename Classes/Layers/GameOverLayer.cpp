#include "GameOverLayer.h"
#include "Jus.h"

USING_NS_CC;

static const std::string FONT_NAME    = "fonts/Bicubik.ttf";
static const std::string TITLE_TEXT   = "Game Over";
static const std::string PREFIX_SCORE = "score: ";
static const std::string PREFIX_BONUS = "bonus: ";
static const std::string PREFIX_EAT   = "eat:   ";
static const std::string TITLE_NO_WINS= "Nobody wins";

static const float   TIME_ACTION_FADE_IN = 0.25f;
static const GLubyte NORMAL_OPACITY      = 0.35f * 0xff;
static const Vec2    ANCHOR_LABEL        = Vec2::ANCHOR_MIDDLE_LEFT;

static const int SZ_FONT_WIN = 32;
static const int SZ_FONT_DEF = 20;
static const int SZ_ADD_KERN = 2 ;

// on "init" you need to initialize your instance
bool GameOverLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if(!Sprite::initWithFile("PregameMenu.png"))
        return false;

    setOpacity(0);

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    runFadeInMainBorder([this] {

    });

    initLabels();
    initSeparator();
    initMenu();

    return true;
}


void GameOverLayer::initLabels()
{

    labelTitle = Jus::createLabelTTF(TITLE_TEXT, FONT_NAME, SZ_FONT_WIN);
    labelTitle->setAdditionalKerning(2);
    labelTitle->setPosition(Jus::getPointNode(this, Vec2(0.5, 1)) - Vec2(0, labelTitle->getContentSize().height / 2));
    addChild(labelTitle);

    const float topy = Jus::getPointNode(this, Vec2(0, 0.7)).y;

    const float scl = _director->getContentScaleFactor();

    for (const auto data : {std::make_pair((int)ID_SNAKE::FIRST, 0.f),
                            std::make_pair((int)ID_SNAKE::SECOND, getContentSize().width / 2)})
    {
        const int id = data.first;
        const float offsetx = data.second;

        labelName[id] = Jus::createLabelTTF("", FONT_NAME, SZ_FONT_DEF);
        labelName[id]->setAdditionalKerning(SZ_ADD_KERN);
        labelName[id]->setAnchorPoint(ANCHOR_LABEL);
        labelName[id]->setPosition(SZ_FONT_DEF + offsetx, topy);
        addChild(labelName[id]);

        labelScore[id] = Jus::createLabelTTF(PREFIX_SCORE, FONT_NAME, SZ_FONT_DEF);
        labelScore[id]->setAdditionalKerning(SZ_ADD_KERN);
        labelScore[id]->setAnchorPoint(ANCHOR_LABEL);
        labelScore[id]->setPosition(SZ_FONT_DEF + offsetx, topy - SZ_FONT_DEF - 10/scl);
        addChild(labelScore[id]);

        labelBonus[id] = Jus::createLabelTTF(PREFIX_BONUS, FONT_NAME, SZ_FONT_DEF);
        labelBonus[id]->setAdditionalKerning(SZ_ADD_KERN);
        labelBonus[id]->setAnchorPoint(ANCHOR_LABEL);
        labelBonus[id]->setPosition(SZ_FONT_DEF + offsetx, topy - SZ_FONT_DEF*2 - 20/scl);
        addChild(labelBonus[id]);

    }

}

void GameOverLayer::initSeparator()
{
    Sprite *separator = Sprite::create("Separator.png");
    separator->setPosition(getContentSize() / 2);
    separator->setScaleY(0.8);

    log("init separator");

    addChild(separator);
}

void GameOverLayer::initMenu()
{
    Sprite *spriteRestart = Sprite::create("MiniRestart.png");
    itemRestart = MenuItemSprite::create(spriteRestart, spriteRestart);
    itemRestart->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    itemRestart->setPosition(SZ_FONT_DEF, SZ_FONT_DEF / 2);

    Sprite *spriteHome = Sprite::create("MiniHome.png");
    itemHome = MenuItemSprite::create(spriteHome, spriteHome);
    itemHome->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    itemHome->setPosition(Vec2(getContentSize().width / 2, SZ_FONT_DEF / 2));

    Sprite *spriteNext = Sprite::create("MiniNext.png");
    itemNext = MenuItemSprite::create(spriteNext, spriteNext);
    itemNext->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    itemNext->setPosition(getContentSize().width - SZ_FONT_DEF, SZ_FONT_DEF / 4);

    menu = Menu::create(itemRestart, itemHome, itemNext, nullptr);
    menu->setPosition(Vec2::ZERO);
//    menu->setPosition(visibleSize / 2 - getContentSize());

    addChild(menu);

    //    Sprite *spriteNext = Sprite::create("")
}

void GameOverLayer::initSpritesBonus()
{
    for (const auto data : {std::make_pair((int)ID_SNAKE::FIRST, 0.f),
                            std::make_pair((int)ID_SNAKE::SECOND, getContentSize().width / 2)})
    {
        const int id = data.first;
        const float offsetx = data.second;
    }
}

void GameOverLayer::runFadeInMainBorder(std::function<void()> callfuntOfEnded)
{
    this->setScale(2);

    auto actionNormalScale   = EaseIn::create(ScaleTo::create(TIME_ACTION_FADE_IN, 1), 3);
    auto actionNormalOpacity = EaseIn::create(FadeTo::create(TIME_ACTION_FADE_IN, NORMAL_OPACITY), 6);

    this->runAction(Spawn::create(actionNormalScale,
                                  actionNormalOpacity,
                                  CallFunc::create([=]{ callfuntOfEnded(); }),
                                  nullptr));
}

void GameOverLayer::showDanceWin(ID_SNAKE id)
{
    std::string text;
    if (id == NO_WINS) {
        text = TITLE_NO_WINS;
    }
    else {
        text += "Win " + _name[id];
    }

    std::string tmp = " ";

    const float size_font = 32;

    const float amplitude = size_font;

    const float w = Jus::createLabelSystem(text, "monospace", size_font)->getContentSize().width;

    float counter = 0;

    std::vector<Label*> labels;
    labels.reserve(text.length() + 1);

    for (int i = 0; i < text.length(); i++) {

        tmp[0] = text[i];
        Label *label = Jus::createLabelSystem(tmp, "monospace", size_font);

        float start = getContentSize().width / 2 - w / 2;

        label->setPosition(start + counter, getContentSize().height * 1.25);
        addChild(label);

        label->setOpacity(0);

        labels.push_back(label);

        counter += label->getContentSize().width + 3;
    }

    const char *schRunningDance = "schRunDance";
    static int index = 0;

    schedule([=, &index] (float) {

        if (index >= labels.size()) {
            index = 0;
            unschedule(schRunningDance);
            return;
        }

        else {
            auto up   = EaseInOut::create(MoveBy::create(0.7, Vec2(0,  amplitude)), 2);
            auto down = EaseInOut::create(MoveBy::create(0.7, Vec2(0, -amplitude)), 2);

            auto seq = Sequence::create(up, down, nullptr);

            labels[index  ]->runAction(FadeIn::create(0.3));
            labels[index++]->runAction(RepeatForever::create(seq));
        }

    }, 1.f / text.size(), schRunningDance);
}

void GameOverLayer::showFireworks()
{
    schedule([this](float){

        if (getParent()) {
            ParticleSystemQuad *firework = ParticleSystemQuad::create("particle_texture.plist");
            firework->setScale(cocos2d::rand_0_1() * 0.3f);
            firework->setPosition(cocos2d::random(visibleSize.width * 0.10, visibleSize.width * 0.90),
                                  cocos2d::random(visibleSize.height* 0.10, visibleSize.height* 0.90));
            getParent()->addChild(firework);

            firework->runAction(Sequence::create(DelayTime::create(5),
                                                 CallFunc::create([firework]{ firework->removeFromParent(); }),
                                                 nullptr));
        }
    }, 0.85, "schfirewrks");
}

void GameOverLayer::setScore(GameOverLayer::ID_SNAKE id, int count)
{
    if (labelScore[id]) {
        _score[id] = count;
        labelScore[id]->setString(PREFIX_SCORE + StringUtils::toString(count));
    }
}

void GameOverLayer::setBonus(GameOverLayer::ID_SNAKE id, int count)
{
    if (labelBonus[id]) {
        _bonus[id] = count;
        labelBonus[id]->setString(PREFIX_BONUS + StringUtils::toString(count));
    }
}

void GameOverLayer::setEat(GameOverLayer::ID_SNAKE id, int count)
{
    if (labelEat[id]) {
        _eat[id] = count;
        labelEat[id]->setString(PREFIX_EAT + StringUtils::toString(count));
    }
}

void GameOverLayer::setSnakeName(GameOverLayer::ID_SNAKE id, const std::string &name)
{
    _name[id] = name;
    labelName[id]->setString(name);
}

void GameOverLayer::setCallbackRestart(const std::function<void()> &callback)
{
    itemRestart->setCallback([callback](Ref*){ callback(); });
}

void GameOverLayer::setCallbackHome(const std::function<void()> &callback)
{
    itemHome->setCallback([callback](Ref*){ callback(); });
}

void GameOverLayer::setCallbackNext(const std::function<void ()> &callback)
{
    itemNext->setCallback([callback](Ref*){ callback(); });
}


//////////////////////////////////////////////////////////////////////////////

void GameOverLayer::BonusSprite::initLabel()
{
    labelCountBonusses = Jus::createLabelTTF("0", FONT_NAME, SZ_FONT_DEF);
    labelCountBonusses->setPosition(getContentSize() / 2);
    addChild(labelCountBonusses);
}

void GameOverLayer::BonusSprite::setCountBonusses(int n)
{
    if (!labelCountBonusses)
        initLabel();

    labelCountBonusses->setString(StringUtils::toString(n));
}

void GameOverLayer::BonusSprite::addCountBonusses(int n)
{
    if (!labelCountBonusses)
        initLabel();

    const int c = Value(labelCountBonusses->getString()).asInt();
    labelCountBonusses->setString(StringUtils::toString(n + c));
}
