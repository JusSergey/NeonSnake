#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "cocos2d.h"
#include <vector>
#include <functional>
#include "Constant.h"

NS_CC_BEGIN

using std::vector;

class Snake : public Layer {

public:
    enum Direction {Left = 0, Right, Up, Down, Zero};

    /* Data */
public:
    Snake  *opponent;
    Sprite *head;

private:
    int indexBackBlock;

protected:
    vector<Sprite*> snakeBlocks;
    Direction directionSnakeHead;
    Color3B colorBlockSnake;
    size_t lenghtSnake;
    size_t countEatBonusses;
    float speedSnake;
    bool isMovingHeadSnake;
    bool isMovingBodySnake;

    /* Setter functions */
public:
    virtual void setPosition (const Vec2 &newPosition);
    virtual void setColor (const Color3B &color);
    virtual void setName (const std::string &name);
    virtual void setTag (int tag);

    void setCountBonusses(size_t n) { countEatBonusses = n; }
    void setSpeed(float speedSnake);
    void setRealLength (const size_t len);
    void setOpponent(Snake *value);

    /* Getter functions */
public:
    inline Direction getDirection     () const { return directionSnakeHead; }
    inline size_t    getLength        () const { return snakeBlocks.size() / stepAddLengthSnake; }
    inline size_t    getCountBonusses () const { return countEatBonusses; }
    inline float     getHeadWidth     () const { return head->getContentSize().width; }
    inline float     getSpeed         () const { return speedSnake; }

    virtual const Vec2& getPosition () const;
    virtual const std::string& getName() const;
    virtual float getPositionX() const { return head->getPositionX(); }
    virtual float getPositionY() const { return head->getPositionY(); }

    virtual int getTag() const;

    Size getCircleSize();
    Snake *getOpponent() const;
    Sprite *getBlock();

    /* else user functions */
public:
    virtual void startMovingHead();
    virtual void stopMovingHead();
    virtual void startMovingBody();
    virtual void stopMovingBody();
    virtual void startAll();
    virtual void stopAll();

    inline bool isMovingHead() const { return isMovingHeadSnake; }
    inline bool isMovingBody() const { return isMovingBodySnake; }
    inline void eatBonus() { countEatBonusses++; }

    void addSnakeBlock (size_t add = 1);
    void subSnake      (int sublen = 1);

    bool isContainer   (const Point &point, float maxDistance) const;

protected:
    Snake();
    virtual bool init();
    virtual void movingHead(float delta) = 0;

    void movingSnakeBody(float f);
    void initLight();
    void initPhysicsBodyHead();
    Sprite *createBlockSnake();

private:
    void addOneBlock();

};

NS_CC_END

#endif // __SNAKE_H__
