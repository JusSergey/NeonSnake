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

    inline void hide() { setPosition(Vec2::ZERO - getContentSize()*3); }

    virtual bool isVisible() const;

    static Bonus* create();

private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    TypeBonusMask type;
    cocos2d::Texture2D* textureFantazyShader;
    cocos2d::Texture2D* textureBigScore;
    cocos2d::Texture2D* textureBomba;
    cocos2d::Texture2D* textureLowSnake;
    cocos2d::Texture2D* textureFastSnake;
    int32_t secondsOfUpdate;

public:
    inline const TypeBonusMask& getBonusType() { return type; }

    void setBonusType(TypeBonusMask type);

protected:
    virtual void eate(Node *nodeContact);

private:
    virtual void setVisible(bool isVisible){ Node::setVisible(isVisible); }
    void initPhysicsBody();
    void initActionBonus();
    void initRandoming();
//    bool isContact(const Vec2 &point, float maxRadius);

};

NS_CC_END

#endif // __BONUS_H__
