#include "NormalEat.h"
#include "Constant.h"

USING_NS_CC;

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

    log("init eat");

    setTexture("eat.png");

    setColor(Color3B::MAGENTA);

    initPhysicsBody();

    initSpriteSwapPosition();

    return true;
}

void NormalEat::initSpriteSwapPosition()
{
    const char *nameSchedule = "initSwapPosition";

    swapImageEat = nullptr;

    schedule([=](float){

        if (getParent() && swapImageEat == nullptr) {

            swapImageEat = Sprite::createWithTexture(getTexture());
            swapImageEat->setCameraMask((unsigned int)CameraFlag::USER1);
            swapImageEat->setPosition(getPosition());
            swapImageEat->setScale(0.1);
            getParent()->addChild(swapImageEat);

            unschedule(nameSchedule);

            this->isInitSwapPosition = true;
        }

    }, updateTimeMSec*10, nameSchedule);
}

void NormalEat::setPosition(const Vec2 &position)
{
    Node::setPosition(position);

    if (isInitSwapPosition) {

        setScale(0.1);

        Node::setPosition(position);
        runAction(ScaleTo::create(timeAnimationEat, 1));

        swapImageEat->setScale(1);
        swapImageEat->runAction(Sequence::create(ScaleTo::create(timeAnimationEat, 0.1),
                                                 CallFunc::create([this](){ swapImageEat->setPosition(getPosition());}),
                                                 nullptr));
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

//void NormalEat::setRandomPosition()
//{

//    Vec2 pos(rand() % (int)visibleSize.width, rand() % (int)visibleSize.height);

//    while (getPosition().distance(pos) < 400)
//        pos.set(rand() % (int)visibleSize.width, rand() % (int)visibleSize.height);

//    setPosition(pos);

//}

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

