#include "BasicEat.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool BasicEat::init()
{
    if(!Sprite::init())
        return false;

    mode = Mode::Active;

    log("initialize BasicEat...");

    blockedContact = false;

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    callfunc = [](Node*){};
    isVerifyCorrectlyPosition = [](const Vec2 &pos) { return true; };

    initContactListener();

    return true;
}

bool BasicEat::isContainer(const PhysicsBody *body, const PhysicsContact &in) const
{
    return (in.getShapeA()->getBody() == body || in.getShapeB()->getBody() == body);
}

Node *BasicEat::getANode(const PhysicsContact &contact) const
{
    return contact.getShapeA()->getBody()->getNode();
}

Node *BasicEat::getBNode(const PhysicsContact &contact) const
{
    return contact.getShapeB()->getBody()->getNode();
}

void BasicEat::setCallbackContact(std::function<void (Node *node)> func)
{
    callfunc = func;
}

void BasicEat::setCallbackVerifyPosition(std::function<bool(const Vec2 &pos)> func)
{
    isVerifyCorrectlyPosition = func;
}

void BasicEat::setRandomPosition()
{
    Vec2 pos;

    int countLoops = 0;

    do pos.set(cocos2d::random(0.f, visibleSize.width), cocos2d::random(0.f, visibleSize.height));
    while (((getPosition().distance(pos) < 200) || !isVerifyCorrectlyPosition(pos)) && countLoops++ < 64);

    setPosition(pos);
}

BasicEat::Mode BasicEat::getMode() const
{
    return mode;
}

void BasicEat::setMode(const Mode &value)
{
    mode = value;
}

void BasicEat::initContactListener()
{
    auto listener = EventListenerPhysicsContact::create();
    
    
    auto callbackContact = [this] (PhysicsContact &contact) -> void {

        if (mode == Mode::Active && isContainer(getPhysicsBody(), contact) && isVisible() && !blockedContact) {

            blockedContact = true;

            runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create( [this](){ blockedContact = false; } ), nullptr));

            auto node = getANode(contact) == this ? getBNode(contact) : getANode(contact);

            callfunc(node); // call custom user function
            eate(node);     // call virtual function

        }
    };


    listener->onContactBegin = [=] (PhysicsContact &contact) -> bool {
        if (mode == Mode::Active)
            callbackContact(contact);

        return true;
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

}
