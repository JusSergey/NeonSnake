#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "cocos2d.h"
#include <vector>
#include <functional>


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
//    size_t maxLengthSnake;
    float speedSnake;
    bool isMovingHeadSnake;
    bool isMovingBodySnake;

    /* Setter functions */
public:
    virtual void setPosition (const Vec2 &newPosition);
    virtual void setColor (const Color3B &color);
    virtual void setName (const std::string &name);
    virtual void setTag (int tag);

    void setSpeed(float speedSnake);

//    inline void setMaxLength(size_t max_length) { this->maxLengthSnake = max_length; }

    /* Getter functions */
public:
    inline Direction getDirection () const { return directionSnakeHead;           }
    inline size_t    getLength    () const { return snakeBlocks.size() - 1;       }
    inline float     getHeadWidth () const { return head->getContentSize().width; }
    inline float     getSpeed     () const { return speedSnake;                   }

    virtual const Vec2& getPosition () const;
    virtual float getPositionX() const { return head->getPositionX(); }
    virtual float getPositionY() const { return head->getPositionY(); }

    virtual const std::string& getName() const;
    virtual int getTag() const;

    /* else user functions */
public:
    virtual void startMovingHead();
    virtual void stopMovingHead();
    virtual void startMovingBody();
    virtual void stopMovingBody();
    virtual void startAll();
    virtual void stopAll();

    void setRealLength (const size_t len);
    void addSnakeBlock (size_t add = 1);
    inline bool isMovingHead() const { return isMovingHeadSnake; }
    inline bool isMovingBody() const { return isMovingBodySnake; }
    bool isContainer   (const Point &point, float maxDistance) const;
    void subSnake      (int sublen = 1);

    Snake *getOpponent() const;
    void setOpponent(Snake *value);

protected:
    Snake();
    virtual bool init();
    virtual void movingHead(float delta) = 0;

protected:
    void movingSnakeBody(float f);
    void initLight();
    void initPhysicsBodyHead();
    Sprite *createBlockSnake();

private:
    void addOneBlock();

};

NS_CC_END

#endif // __SNAKE_H__
