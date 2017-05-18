#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Snake.h"

NS_CC_BEGIN

static const Vec2 DescriptionWay[] = { Vec2(-1,  0),
                                       Vec2( 1,  0),
                                       Vec2( 0,  1),
                                       Vec2( 0, -1),
                                       Vec2::ZERO };

//////////////////////////

class Player : public Snake {

public:
    EventKeyboard::KeyCode keyLeft;
    EventKeyboard::KeyCode keyRight;
    EventKeyboard::KeyCode keyUp;
    EventKeyboard::KeyCode keyDown;
    EventKeyboard::KeyCode keyStop;

protected:
    virtual void movingHead(float);
    virtual void stopMovingHead();

private:
    virtual bool init();
    void initKeyboard();
    void initTouches();

    Direction directionSnakeHead;
    Rect touchRect;

public:
    void setDirectionSnakeHead(Snake::Direction direct);
    void setTouchRect(const Rect &rect);

public:
    CREATE_FUNC(Player)

};

NS_CC_END

#endif // __PLAYER_H__
