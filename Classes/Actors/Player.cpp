#include "Player.h"
#include "Constant.h"

USING_NS_CC;

bool Player::init()
{
    log("initialize Player...");

    if (!Snake::init())
        return false;

    directionSnakeHead = Snake::Direction::Zero;

    auto sz = Director::getInstance()->getVisibleSize();

    touchRect.setRect(0, 0, sz.width, sz.height);

    keyLeft  = EventKeyboard::KeyCode::KEY_LEFT_ARROW;
    keyRight = EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
    keyUp    = EventKeyboard::KeyCode::KEY_UP_ARROW;
    keyDown  = EventKeyboard::KeyCode::KEY_DOWN_ARROW;
    keyStop  = EventKeyboard::KeyCode::KEY_SPACE;

    initKeyboard();
    initTouches();

    setTag(TagPlayer);

    return true;
}

void Player::initKeyboard()
{
    auto listenerKeyboard = EventListenerKeyboard::create();

    listenerKeyboard->onKeyPressed = [this] (EventKeyboard::KeyCode pressed, Event*) -> void {

        if (pressed == keyLeft) {
            setDirectionSnakeHead(Direction::Left);
            return;
        }
        if (pressed == keyRight) {
            setDirectionSnakeHead(Direction::Right);
            return;
        }
        if (pressed == keyUp) {
            setDirectionSnakeHead(Direction::Up);
            return;
        }
        if (pressed == keyDown) {
            setDirectionSnakeHead(Direction::Down);
            return;
        }
        if (pressed == keyStop) {
            setDirectionSnakeHead(Direction::Zero);
            return;
        }
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerKeyboard, this);

}

void Player::initTouches()
{
    auto lTouch = EventListenerTouchOneByOne::create();

    lTouch->onTouchBegan = [this](Touch *touch, Event *) -> bool {
        return true;
    };

    lTouch->onTouchEnded = [=](Touch *touch, Event *) -> void {

        log("ended");

        if (!touchRect.containsPoint(touch->getStartLocation()))
            return;

        log("post ended");

        Vec2 start  = touch->getStartLocation();
        Vec2 finish = touch->getLocation();

        if (start.distance(finish) < 5) {
            setDirectionSnakeHead(Snake::Direction::Zero);
            return;
        }

        float x = start.x - finish.x;
        float y = start.y - finish.y;

        if (abs(x) > abs(y)) {
            if (x < 0) setDirectionSnakeHead(Snake::Direction::Right);
            else       setDirectionSnakeHead(Snake::Direction::Left);
        }
        else {
            if (y < 0) setDirectionSnakeHead(Snake::Direction::Up);
            else       setDirectionSnakeHead(Snake::Direction::Down);
        }
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lTouch, this);
}

void Player::setDirectionSnakeHead(Snake::Direction direct)
{
    if (Director::getInstance()->isPaused())
        return;

    directionSnakeHead = direct;

}

void Player::setTouchRect(const Rect &rect)
{
    touchRect = rect;
}

void Player::movingHead(float)
{
    PhysicsBody *pBody = head->getPhysicsBody();

    if (pBody)
        pBody->setVelocity(DescriptionWay[directionSnakeHead] * speedSnake * discret);

}
