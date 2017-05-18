#ifndef __EAT_H__
#define __EAT_H__

#include "cocos2d.h"
#include <functional>

class BasicEat : public cocos2d::Sprite
{
public:
    enum Tags{
        TagBasicEat = 0xA,
        TagBasicBonus
    };
    enum Mode {
        Active,
        Passive
    };

public:
    virtual bool init();

    bool isContainer(const cocos2d::PhysicsBody *body, const cocos2d::PhysicsContact& in) const;

    cocos2d::Node *getANode(const cocos2d::PhysicsContact &contact) const;
    cocos2d::Node *getBNode(const cocos2d::PhysicsContact &contact) const;

    void setCallbackContact(const std::function<void(Node *node)> &func);
    void setCallbackVerifyPosition(const std::function<bool(const cocos2d::Vec2 &pos)> &func);
    void setRandomPosition();

    Mode getMode() const;
    void setMode(const Mode &value);

    virtual void shutdown() = 0;

protected:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    Mode mode;

protected:
    virtual void eate(Node *nodeContact) = 0;

private:
    void initContactListener();

protected:
    std::function<void(Node *node)> callfunc;
    std::function<bool(const cocos2d::Vec2 &pos)> isVerifyCorrectlyPosition;

};

#endif // __EAT_H__
