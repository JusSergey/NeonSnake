#ifndef __JUS_H__
#define __JUS_H__

#include "cocos2d.h"
NS_CC_BEGIN
namespace Jus {

static float scW;
static float scH;
void setScaledInit(cocos2d::Size SizeFrame);

void setScale(cocos2d::Node *node, cocos2d::PhysicsBody *body = nullptr);
void setScaledPosition(cocos2d::Node *node, cocos2d::Vec2 pos);

cocos2d::Vec2 getDisplayPoint(float x, float y);
cocos2d::Vec2 getDisplayPoint(const Vec2 &vec);
cocos2d::Vec2 getPointNode(Node *node, const Vec2 &vec);
cocos2d::Vec2 getCenter();
cocos2d::Size getSize();
cocos2d::Size getSize(cocos2d::Node *node);

inline float scaledDisplayW(float w);
inline float scaledDisplayH(float h);

cocos2d::Size scaledDisplay(float w, float h);

float getWidth();
float getHeight();

void addVelocity(cocos2d::PhysicsBody *body, const cocos2d::Vec2 &vec);
void addAngular(PhysicsBody *body, float angular);
void addPosition(Node *node, const Vec2 &vec);

}
NS_CC_END
#endif // __JUS_H__
