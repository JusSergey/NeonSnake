#include "BasicEat.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool BasicEat::init()
{
    if(!Sprite::init())
        return false;

    mode = Mode::Active;

    log("initialize BasicEat...");

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    callfunc = [](Node*){};
    isVerifyCorrectlyPosition = [](const Vec2 &pos) { return true; };

    initContactListener();

    return true;
}

bool BasicEat::isContainer(const PhysicsBody *body, const PhysicsContact &in) const {
    return (in.getShapeA()->getBody() == body || in.getShapeB()->getBody() == body);
}

Node *BasicEat::getANode(const PhysicsContact &contact) const {
    return contact.getShapeA()->getBody()->getNode();
}

Node *BasicEat::getBNode(const PhysicsContact &contact) const {
    return contact.getShapeB()->getBody()->getNode();
}

void BasicEat::setCallbackContact(const std::function<void (Node *node)> &func) {
    callfunc = func;
}

void BasicEat::setCallbackVerifyPosition(const std::function<bool(const Vec2 &pos)> &func) {
    isVerifyCorrectlyPosition = func;
}

void BasicEat::setRandomPosition() {

    Vec2 pos;

    int countLoops = 0;

    do pos.set(cocos2d::random(0.f, visibleSize.width), cocos2d::random(0.f, visibleSize.height));
    while (((getPosition().distance(pos) < 200) || !isVerifyCorrectlyPosition(pos)) && countLoops++ < 64);

    setPosition(pos);
}

BasicEat::Mode BasicEat::getMode() const {
    return mode;
}

void BasicEat::setMode(const Mode &value) {
    mode = value;
}

void BasicEat::shutdown()
{
    removeFromParent();
}

void BasicEat::initContactListener()
{
    auto listener = EventListenerPhysicsContact::create();
    
    auto callbackContact = [this] (PhysicsContact &contact) -> void {

//        log("CONTACT_BASIC_EAT[%d][%d][%d]", getPhysicsBody(), contact.getShapeA()->getBody(), contact.getShapeB()->getBody());

        if (isContainer(getPhysicsBody(), contact)/* && isVisible()*/) {
//            log("INSIDE_IF");
            auto node = getANode(contact) == this ? getBNode(contact) : getANode(contact);
            eate(node);     // call virtual function
            callfunc(node); // call custom user function
        }
//        else log("OUTSIDE_IF[%d]", isVisible());
    };


    listener->onContactBegin = [=] (PhysicsContact &contact) -> bool {
        if (mode == Mode::Active)
            callbackContact(contact);

        return true;
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

}
