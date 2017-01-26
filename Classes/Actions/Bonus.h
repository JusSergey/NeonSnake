#ifndef __BONUS_H__
#define __BONUS_H__

#include "cocos2d.h"
#include "BasicEat.h"

NS_CC_BEGIN

class Bonus : public BasicEat
{
private:

public:
    enum TypeBonusMask {
        FastSnake     = 1 << 0,
        LowSnake      = 1 << 1,
        FantazyShader = 1 << 2,
        BigScore      = 1 << 3,
        Bomba         = 1 << 4
    };

public:

    void setRandomBonus();

    virtual bool init();

    static Bonus* create(int type);

private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    TypeBonusMask type;

public:
    inline const TypeBonusMask& getBonusType() { return type; }

protected:
    virtual void eate(Node *nodeContact);

private:
    void initPhysicsBody();
    void initActionBonus();
//    bool isContact(const Vec2 &point, float maxRadius);

};

NS_CC_END

#endif // __BONUS_H__
