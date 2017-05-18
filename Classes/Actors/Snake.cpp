#include "Snake.h"
//#include "Constant.h"
#include "cocos2d.h"
#include <functional>
#include "Constant.h"

//static const char *scheduleRunningBodyName = "runningBody";
static const char *scheduleRunningHeadName = "runningHead";
static const char *pathSnakeBlock = "circle.png";


USING_NS_CC;
static bool isFirstInit = true;

static const Vec2 DescriptionWay[] = { Vec2(-1,  0),
                                       Vec2( 1,  0),
                                       Vec2( 0,  1),
                                       Vec2( 0, -1),
                                       Vec2::ZERO };

using namespace std;

Snake::Snake() : Layer(),
    directionSnakeHead(Direction::Zero),
    lenghtSnake(0),
    countEatBonusses(0),
    speedSnake(8 / _director->getContentScaleFactor()),
    isMovingHeadSnake(false),
    colorBlockSnake(Color3B::WHITE),
    opponent(nullptr)
{
    if (isFirstInit) {
        TextureCache::getInstance()->addImage(pathSnakeBlock);
        isFirstInit = false;
    }

    snakeBlocks.reserve(128);

}

bool Snake::init()
{
    if (!Layer::init())
        return false;

    log("initialize Snake");

    initLight();
    addSnakeBlock(DefaultLenghtSnake);
    head = snakeBlocks[0];

    indexBackBlock = snakeBlocks.size()-1;

    schedule(schedule_selector(Snake::movingSnakeBody), updateTimeMSec);

    initPhysicsBodyHead();

    schedule([=](float){
        Snake::setSpeed(getSpeed() + 1);
        log("speed: %f", getSpeed());
    }, IntervalAddingSpeedSnake, "addSpeed");

    return true;
}

void Snake::movingSnakeBody(float f)
{
    if (indexBackBlock <= 0 || indexBackBlock >= snakeBlocks.size())
        indexBackBlock = snakeBlocks.size()-1;

    if (snakeBlocks.size())
        snakeBlocks[indexBackBlock]->setPosition(/*head->*/getPosition());

    indexBackBlock--;
}

void Snake::setName(const string &name)
{
    head->setName(name);
}

const string& Snake::getName() const
{
    return head->getName();
}

int Snake::getTag() const
{
    return head->getTag();
}

Size Snake::getCircleSize()
{
    return head->getContentSize();
}

void Snake::setTag(int tag)
{
    head->setTag(tag);
}

void Snake::startMovingHead()
{
    if (!isMovingHeadSnake) {
        isMovingHeadSnake = true;
        schedule(schedule_selector(Snake::movingHead), 1.f / speedSnake);
    }
}

void Snake::stopMovingHead()
{
    head->stopAllActions();

    if (isScheduled(schedule_selector(Snake::movingHead))) {
        isMovingHeadSnake = false;
        unschedule(schedule_selector(Snake::movingHead));
    }
}

void Snake::startMovingBody()
{
    if (!isMovingBodySnake) {
        isMovingBodySnake = true;
        schedule(schedule_selector(Snake::movingSnakeBody), updateTimeMSec);
    }
}

void Snake::stopMovingBody()
{
    if (isScheduled(schedule_selector(Snake::movingSnakeBody))) {
        isMovingBodySnake = false;
        unschedule(schedule_selector(Snake::movingSnakeBody));
    }
}

void Snake::startAll()
{
    startMovingHead();
    startMovingBody();
}

void Snake::stopAll()
{
    stopMovingHead();
    stopMovingBody();
}

void Snake::setRealLength(const size_t len)
{
    if (getLength() == len)
        return;

    int direct = getLength() > len ? -1 : 1;

    for (int i = getLength(); i != len; i += direct) {
        addOneBlock();
    }
}

void Snake::setSpeed(float speed)
{
    if (speed >= MaxSnakeSpeed)
        speedSnake = MaxSnakeSpeed;

    else if (speed <= MinSnakeSpeed)
        speedSnake = MinSnakeSpeed;

    else speedSnake = speed;

    if (isMovingHeadSnake){
        stopMovingHead();
        startMovingHead();
    }
}

void Snake::setColor(const Color3B &color)
{
    colorBlockSnake = color;

    for (const auto &i : snakeBlocks)
        i->setColor(colorBlockSnake);
}

Snake *Snake::getOpponent() const
{
    return opponent;
}

void Snake::setOpponent(Snake *value)
{
    opponent = value;
}

Sprite *Snake::getBlock()
{
    int getIndex = snakeBlocks.size() - 1;
    Sprite *retBlock = nullptr;

    if (getIndex > 0 && !snakeBlocks.empty()) {
        retBlock = snakeBlocks[getIndex];
        snakeBlocks.pop_back();
//        for (int i = indexBackBlock-1; i >= 0 && i > (indexBackBlock - stepAddLengthSnake); i--) {
//            log("rin");
//            snakeBlocks[i]->removeFromParent();
//            log("rout");
//            snakeBlocks.pop_back();
//        }
//        if (snakeBlocks.size() > 0)
//            snakeBlocks.pop_back();

    }

    return retBlock;
}

void Snake::setPosition(const Vec2 &newPosition)
{
    for (auto obj : snakeBlocks)
        obj->setPosition(newPosition);
}

const Vec2 &Snake::getPosition() const
{
    return head->getPosition();
}

void Snake::addSnakeBlock(size_t add)
{
    if ((lenghtSnake + add*stepAddLengthSnake) > MaxLenSnake) {
        add = MaxLenSnake - lenghtSnake;
        if (add <= 0)
            return;
    }

    lenghtSnake += add;

    for (int i = 0; i < add*stepAddLengthSnake; i++) {
        addOneBlock();
    }

}

bool Snake::isContainer(const Point &point, float maxDistance) const
{
    for (const auto &i : snakeBlocks)
        if (i->getPosition().distance(point) <= maxDistance)
            return true;

    return false;
}

void Snake::subSnake(int sublen)
{
    sublen *= stepAddLengthSnake;
    for (int i = snakeBlocks.size()-1; i > 0 && sublen-- > 0; i--){
        removeChild(snakeBlocks[i]);
        snakeBlocks.pop_back();
    }

}

void Snake::initLight()
{
    static int light_id = 1;

    Sprite *light = Sprite::create("light.png");

    light->setScale(0.40);
    light->setCameraMask((unsigned int)CameraFlag::USER1);

    addChild(light, LLight);

    schedule([=](float){
        light->setPosition(head ? head->getPosition() : Vec2::ZERO - light->getContentSize());
    }, updateTimeMSec, (std::string("snakeLight") + StringUtils::toString(light_id)).c_str());

    light_id++;
}

void Snake::initPhysicsBodyHead()
{
    // max supported snakes, at 32
    static int8_t count(0);

    auto body = PhysicsBody::createCircle(getHeadWidth() * 0.40, PhysicsMaterial(0, 0, 0));

    body->setMass(SnakeMass);
    body->setDynamic(true);
    body->setRotationEnable(false);

    body->setContactTestBitmask (TestBitmask::SnakeBitMask << count);
    body->setCollisionBitmask   (TestBitmask::SnakeBitMask << count);
    body->setCategoryBitmask    (TestBitmask::SnakeBitMask << count);

    head->setPhysicsBody(body);
    count++;
}

Sprite *Snake::createBlockSnake()
{
    return Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey(pathSnakeBlock));
}

void Snake::addOneBlock()
{
    auto block = createBlockSnake();
    block->setColor(colorBlockSnake);

    block->setOpacity(0xff * OpacitySnakePercent);

    block->setCameraMask((unsigned int)CameraFlag::USER1);

    this->addChild(block);

    if (snakeBlocks.empty()) {
        block->setPosition(100, 100);
    }

    else block->setPosition(snakeBlocks[snakeBlocks.size()-1]->getPosition());

    snakeBlocks.push_back(block);
}
