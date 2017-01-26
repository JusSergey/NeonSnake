#include "Jus.h"

USING_NS_CC;

Vec2 Jus::getDisplayPoint(float x, float y) {
    Size visible = Director::getInstance()->getVisibleSize();
    Vec2 origin  = Director::getInstance()->getVisibleOrigin();
    return Vec2(visible.width*x + origin.x, visible.height*y + origin.y);
}

Vec2 Jus::getDisplayPoint(const Vec2 &vec){
    return Jus::getDisplayPoint(vec.x, vec.y);
}

Vec2 Jus::getCenter() {
    return getDisplayPoint(0.5f, 0.5f);
}

Size Jus::getSize() {
    return Director::getInstance()->getVisibleSize();
}

float Jus::getWidth() {
    return Director::getInstance()->getVisibleSize().width;
}

float Jus::getHeight() {
    return Director::getInstance()->getVisibleSize().height;
}

Size Jus::getSize(Node *node)
{
    return node->getContentSize();
}

void Jus::addVelocity(PhysicsBody *body, const Vec2 &vec)
{
    body->setVelocity(body->getVelocity()+vec);
}

float Jus::scaledDisplayW(float w)
{
    return (Director::getInstance()->getVisibleSize().width / w);
}

float Jus::scaledDisplayH(float h)
{
    log("scaledH %f, to %f", h, Director::getInstance()->getVisibleSize().height);
    return (Director::getInstance()->getVisibleSize().height / h);
}

Size Jus::scaledDisplay(float w, float h)
{
    return Size(Jus::scaledDisplayW(w), Jus::scaledDisplayH(h));
}

void Jus::setScale(Node *node, PhysicsBody *body)
{
    if(node){
        node->setScale(Jus::scW, Jus::scH);
    } else log("Node == nullptr");
}

void Jus::setScaledInit(Size SizeFrame)
{
    Jus::scW =  cocos2d::Director::getInstance()->getVisibleSize().width / SizeFrame.width;
    Jus::scH =  cocos2d::Director::getInstance()->getVisibleSize().height / SizeFrame.height;
}

void Jus::setScaledPosition(Node *node, Vec2 pos)
{
    node->setPosition(pos.x*Jus::scW, pos.y*Jus::scH);
}

void Jus::addAngular(PhysicsBody *body, float angular)
{
    body->setAngularVelocity(body->getAngularVelocity() + angular);
}

void Jus::addPosition(Node *node, const Vec2 &vec)
{
    node->setPosition(node->getPosition()+vec);
}

Vec2 Jus::getPointNode(Node *node, const Vec2 &vec)
{
    Size visible = node->getContentSize();
    Vec2 origin  = Director::getInstance()->getVisibleOrigin();
    return Vec2(visible.width*vec.x + origin.x, visible.height*vec.y + origin.y);
}
