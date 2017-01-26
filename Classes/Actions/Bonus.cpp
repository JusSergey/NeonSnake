#include "Bonus.h"
#include "Constant.h"

USING_NS_CC;

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

    setVisible(false);

    setRandomBonus();

    initActionBonus();

    schedule([this](float){

        setVisible(true);

        scheduleOnce([this](float){
            setVisible(false);
            setRandomPosition();
            setRandomBonus();
        }, 7, "hideBonus");

    }, cocos2d::random(15, 30), "eateSch");

    initPhysicsBody();

    return true;
}

Bonus *Bonus::create(int type)
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

void Bonus::eate(Node *node)
{
    setVisible(false);
}

void Bonus::initPhysicsBody()
{
    if (!getPhysicsBody()) {
        auto *body = PhysicsBody::createCircle(getContentSize().width / 2, PhysicsMaterial(0, 0, 0));
        body->setDynamic(true);
        body->setMass(0.00001);
        body->setContactTestBitmask(TestBitmask::EatBitMask);
        body->setCollisionBitmask(0);
        body->setCategoryBitmask(TestBitmask::EatBitMask);
        setPhysicsBody(body);
    }
}

void Bonus::initActionBonus()
{
    float timeAction = 0.3;

    auto A1 = ScaleTo::create(timeAction, 1.3);
    auto A2 = ScaleTo::create(timeAction, 1);
    auto A3 = Sequence::create(A1, A2, nullptr);

    runAction(RepeatForever::create(A3));
}

void Bonus::setRandomBonus()
{
    TypeBonusMask variable[] {FantazyShader, BigScore, Bomba, LowSnake, FastSnake};

    // set random type
    {
        auto oldType = type;
        while (oldType == (type = variable[random(0, int(sizeof(variable) / sizeof(variable[0]))-1)]));
    }

    switch (type) {
    case TypeBonusMask::FantazyShader: setTexture("Bonus.png");  break;
    case TypeBonusMask::BigScore:      setTexture("5Balls.png"); break;
    case TypeBonusMask::Bomba:         setTexture("Bomba.png");  break;
    case TypeBonusMask::LowSnake:      setTexture("BonusSpeed-.png"); break;
    case TypeBonusMask::FastSnake:     setTexture("BonusSpeed+.png"); break;
    default: log("fail setrbonus");
    }
}
