#include "Bonus.h"
#include "Constant.h"

USING_NS_CC;

static const char *schDelayBetweenBonusses = "schUB";
static const char *schIntervalShowingBonus = "schDM";

// on "init" you need to initialize your instance
bool Bonus::init()
{
    //////////////////////////////
    // 1. super init first
    if(!BasicEat::init())
        return false;

    log("initialize Bonus...");

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    setPosition(Vec2::ZERO - getContentSize());

    textureFantazyShader = _director->getTextureCache()->addImage("Bonus.png");
    textureBigScore = _director->getTextureCache()->addImage("5Balls.png");
    textureBomba = _director->getTextureCache()->addImage("Bomba.png");
    textureLowSnake = _director->getTextureCache()->addImage("BonusSpeed-.png");
    textureFastSnake = _director->getTextureCache()->addImage("BonusSpeed+.png");

    setRandomBonus();

    initActionBonus();

    initRandoming();

    initPhysicsBody();

    return true;
}

bool Bonus::isVisible() const
{
    const Size &sz = getContentSize();
//    log("BONUS[%f, %f] sz[%f, %f]", getPositionX(), getPositionY(), sz.width, sz.height);
    return (getPositionX() > sz.width/2 && getPositionY() > sz.height/2);
}

void Bonus::shutdown()
{
    unschedule(schDelayBetweenBonusses);
    unschedule(schIntervalShowingBonus);
    hide();
}

Bonus *Bonus::create()
{
    Bonus *pRet = new(std::nothrow) Bonus();
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

void Bonus::setBonusType(TypeBonusMask type)
{
    if (particle) {
        particle->removeFromParent();
        particle = nullptr;
    }

    switch (type) {
    case TypeBonusMask::FantazyShader: setTexture(PATH_FANTAZY_SHADER);  break;
    case TypeBonusMask::BigScore:      setTexture(PATH_5BALLS); break;
    case TypeBonusMask::Bomba:         setTexture(PATH_BOMBA);  break;
    case TypeBonusMask::LowSnake:      setTexture(PATH_SPEED_MINUS); break;
    case TypeBonusMask::FastSnake:     setTexture(PATH_SPEED_PLUS); break;
    default: log("Bonus::setBonusType: <undefined bonus type>"); return;
    }

    this->type = type;
}

void Bonus::eate(Node *node)
{
    unschedule(schIntervalShowingBonus);
    secondsOfUpdate = 0;
    hide();
}

void Bonus::initPhysicsBody()
{
    if (!getPhysicsBody()) {
        auto *body = PhysicsBody::createCircle(getContentSize().width / 2, PhysicsMaterial(0, 0, 0));
        body->setDynamic(false);
        body->setMass(0.00001);
        body->setContactTestBitmask(TestBitmask::EatBitMask);
        body->setCollisionBitmask(0);
        body->setCategoryBitmask(TestBitmask::EatBitMask);
        setPhysicsBody(body);
    }
}

void Bonus::initActionBonus()
{
    const float timeAction = 0.3;

    auto A1 = ScaleTo::create(timeAction, 1.3);
    auto A2 = ScaleTo::create(timeAction, 1);
    auto A3 = Sequence::create(A1, A2, nullptr);

    runAction(RepeatForever::create(A3));
}

void Bonus::initRandoming()
{
    secondsOfUpdate = 0;

    schedule([this](float){

        if (++secondsOfUpdate > delayBetweenBonusses/4 && !isVisible()) {
            setRandomPosition();
            setRandomBonus();
            secondsOfUpdate = 0;

            scheduleOnce([this](float){
                secondsOfUpdate = 0;
                hide();
            }, intervalShowingBonus, schIntervalShowingBonus);
        }

    }, 1, schDelayBetweenBonusses);
}

void Bonus::setRandomBonus()
{
    const TypeBonusMask variable[] {FantazyShader, BigScore, Bomba, LowSnake, FastSnake};

    // set random type
    {
        const auto oldType = type;
        while (oldType == (type = variable[random(0, int(sizeof(variable) / sizeof(variable[0]))-1)]));        
    }

    setBonusType(type);
}
