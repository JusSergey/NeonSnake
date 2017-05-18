#ifndef __NORMALEAT_H__
#define __NORMALEAT_H__

#include "cocos2d.h"
#include "BasicEat.h"

class NormalEat : public BasicEat
{
public:
    virtual bool init();

//static NormalEat* create(cocos2d::Node *node)
//{
//    NormalEat *pRet = new(std::nothrow) NormalEat();
//    if (pRet && pRet->init())
//    {
//        pRet->autorelease();
//        return pRet;
//    }
//    else
//    {
//        delete pRet;
//        pRet = nullptr;
//        return nullptr;
//    }
//}
    CREATE_FUNC(NormalEat)

private:
//    NormalEat(cocos2d::Node *node) : BasicEat(node) {}

private:
    cocos2d::Sprite *swapImageEat;
    bool isInitSwapPosition;

private:
    void initSpriteSwapPosition();

public:
    virtual void setPosition(const cocos2d::Vec2 &position);
    virtual void setPositionX(float x);
    virtual void setPositionY(float y);
    virtual void shutdown();
    void initPhysicsBody();
    virtual void eate(Node *nodeContact);

};

#endif // __NORMALEAT_H__
