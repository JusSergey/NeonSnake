#include "NormalEat.h"
#include "Constant.h"

USING_NS_CC;

//static const char *nameSchedule = "InitswpPs";

// on "init" you need to initialize your instance
bool NormalEat::init()
{
    //////////////////////////////
    // 1. super init first
    if(!BasicEat::init())
        return false;

    log("initialize NormalEat");

    srand(time(0));

    isInitSwapPosition = false;
    swapImageEat = nullptr;

    log("init eat");

    setTexture("eat.png");

    setColor(Color3B::MAGENTA);

    initPhysicsBody();

    initSpriteSwapPosition();

    return true;
}

void NormalEat::initSpriteSwapPosition()
{
    schedule([=](float){

        if (getParent() && swapImageEat == nullptr) {

            swapImageEat = Sprite::createWithTexture(getTexture());
            swapImageEat->setCameraMask((unsigned int)CameraFlag::USER1);
            swapImageEat->setPosition(getPosition());
            swapImageEat->setScale(0);
            getParent()->addChild(swapImageEat);

            unschedule("schswpps");

            this->isInitSwapPosition = true;
        }

    }, updateTimeMSec*10, "schswpps");
}

void NormalEat::setPosition(const Vec2 &position)
{
    Node::setPosition(position);

    if (isInitSwapPosition) {

        setScale(0.1);

        Node::setPosition(position);
        runAction(ScaleTo::create(timeAnimationEat, 0.9));

        swapImageEat->setScale(1);

        auto scale = ScaleTo::create(timeAnimationEat, 0);
        swapImageEat->runAction(scale);

        // swapImageEat->runAction(Sequence::create(scale, callfunc, nullptr)); - not stable working.
        // it a fix this bug.

        scheduleOnce([this](float){
            swapImageEat->setPosition(getPosition());
        }, timeAnimationEat, "tmanim");

    }
}

void NormalEat::setPositionX(float x)
{
    setPosition(Vec2(x, getPositionY()));
}

void NormalEat::setPositionY(float y)
{
    setPosition(Vec2(getPositionX(), y));
}

void NormalEat::shutdown()
{
//    unschedule(nameSchedule);
    setPosition(getContentSize() * -3);
}

void NormalEat::initPhysicsBody()
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

void NormalEat::eate(Node *nodeContact)
{
}

