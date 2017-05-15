#include "Bot.h"

USING_NS_CC;

using std::vector;

static const bool wall   = true;
static const bool nowall = false;

typedef Bot::CreateWay::WallsMap Block;
// on "init" you need to initialize your instance
bool Bot::init()
{
    log("initialize Bot...");

    if(!Snake::init())
        return false;

    debugDraw = nullptr;
    bonus = nullptr;
    eat = nullptr;

    indexPoint = 0;

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    setPosition({300, 150});

    setTag(TagBot);

    schedule(schedule_selector(Bot::analizeSituation), 1.f / 10);

    return true;
}

void Bot::initDebugDraw()
{
    static const char *schName = "initdnode";

    if (isScheduled(schName))
        return;

    schedule([&](float){
        if (getParent()) {
            debugDraw = DrawNode::create(3);
            getParent()->addChild(debugDraw, Levels::LTop);
            unschedule(schName);
            log("init debug draw");
        }
    }, 1, schName);
}

void Bot::repaintDebug()
{
    if (debugDraw && debugDraw->getParent() && enableDebugDraw) {
        debugDraw->clear();
        for (const Bot::CreateWay::Int16Point &vec : pointsPath)
            debugDraw->drawPoint(vec.toVec2() * discret, 3, Color4F::RED);
    }
}

bool Bot::getEnableDebugDraw() const
{
    return enableDebugDraw;
}

void Bot::setEnableDebugDraw(bool value)
{
    enableDebugDraw = value;
    if (enableDebugDraw && !debugDraw)
        initDebugDraw();

}

void Bot::movingHead(float delta)
{
    if (indexPoint < pointsPath.size()) {
        auto action = MoveTo::create(delta, pointsPath[indexPoint].toVec2() * discret);
        head->runAction(action);
        int step = speedSnake / 5;

        if (indexPoint + step >= pointsPath.size())
            step = pointsPath.size() - indexPoint - 1;

        indexPoint += (step > 0 ? step : 1);

    }

}

void Bot::start()
{
    isMovingHeadSnake = true;
    int step = speedSnake / 5;
    schedule(schedule_selector(Bot::movingHead), 1.f / speedSnake * (step > 0 ? step : 1));
}



void Bot::analizeSituation(float)
{
    if ((bonus && bonus->getPosition() != analizeData.prevPosBonus) ||
        (eat   && eat->getPosition()   != analizeData.prevPosEat) ||
        (bonus && bonus->isVisible()   != analizeData.visibleBonus) ||
        (pointsPath.empty()))
    {

        log("analize update calculate paths.");

        std::vector<CreateWay::Int16Point> pathToBonus;
        std::vector<CreateWay::Int16Point> pathToEat;

        if (bonus) pathToBonus = this->createway(getPosition(), bonus->getPosition());
        if (eat)   pathToEat   = this->createway(getPosition(), eat->getPosition());

        // якщо до бонуса кортше, і він є видимим
        if (bonus && pathToBonus.size() && pathToBonus.size() < pathToEat.size() && bonus->isVisible()) {
            // і зараз не йде до нього
            if (analizeData.tagObject != BasicEat::TagBasicBonus || !indexPoint || bonus->getPosition() != analizeData.prevPosBonus) {
                analizeData.tagObject  = BasicEat::TagBasicBonus;
                GoTo = bonus->getPosition();
                setPointsPath(pathToBonus);
//                pointsPath = pathToBonus;
                indexPoint = 0;
            }
        }
        else if(eat) {
            if (analizeData.tagObject != BasicEat::TagBasicEat || !indexPoint || eat->getPosition() != analizeData.prevPosEat) {
                analizeData.tagObject  = BasicEat::TagBasicEat;
                GoTo = eat->getPosition();
                setPointsPath(pathToEat);
//                pointsPath = pathToEat;
                indexPoint = 0;
            }
        }

        if (bonus) {
            analizeData.prevPosBonus = bonus->getPosition();
            analizeData.visibleBonus = bonus->isVisible();
        }

        if (eat) {
            analizeData.prevPosEat   = eat->getPosition();
        }

    }

}

std::vector<Bot::CreateWay::Int16Point> Bot::createway(const Vec2 &in, const Vec2 &to)
{
    return CreateWay::getInstance()->createPointsPath(walls, in / discret, to / discret);
}

std::vector<Bot::CreateWay::Int16Point> Bot::createway(const Vec2 &to)
{
    return CreateWay::getInstance()->createPointsPath(walls, getPosition() / discret, to / discret);
}

void Bot::setPointsPath(const std::vector<CreateWay::Int16Point> &value)
{
    pointsPath = value;
    repaintDebug();
}


/////////////////////////////////////////////////////////////////////////////////////////

void Bot::CreateWay::addRect(){

    static const Int16Point paths[] = {{-1, 0},
                                       {0, -1},
                                       {1, 0},
                                       {0, 1},
                                       /*{-1, -1},
                                       {-1, 1},
                                       {1, 1},
                                       {1, -1}*/};

    cycle = false;
    vector<Int16Point> tmpPoints;
    tmpPoints.reserve(points.size()*2);

    for (auto &iterator : points) {
        for (auto &path : paths) {

            int pointX = iterator.x + path.x;
            int pointY = iterator.y + path.y;

            if ((pointX >= 0 && pointX < mapx) && (pointY >= 0 && pointY < mapy) && (blockMap[pointX][pointY].symbol == nowall)) {

                if (!cycle) cycle = true;

                tmpPoints.push_back(Int16Point{pointX, pointY});
                blockMap[pointX][pointY].symbol = wall;
                blockMap[pointX][pointY].parent = iterator;

                if (pointX == end.x && pointY == end.y) {
                    isCreated = true;
                    return;

                }
            }
        }
    }

    points = std::move(tmpPoints);

}


void Bot::CreateWay::control(){

    points.push_back(begin);

    while(cycle && !isCreated)  // пока не зацикливается и не создан путь
        addRect();

    parrent();

}

void Bot::CreateWay::buildRoute(){

    cycle     = true;
    isCreated = false;

    points.clear();
    outResult.clear();

    control();

}

void Bot::CreateWay::parrent(){

    if(!isCreated)
        return;

    outResult.clear();

    Int16Point point = end;

    while(point != begin){
        outResult.push_back(point);
        point = blockMap[point.x][point.y].parent;
    }

    outResult.push_back(begin);

}



bool Bot::CreateWay::isNoWalls(Bot::CreateWay::Int16Point p)
{
    static const Int16Point args[] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

    for (const Int16Point &i : args)
        if (blockMap[p.x + i.x][p.y + i.y].symbol == wall)
            return false;

    return true;
}

Bot::CreateWay *Bot::CreateWay::getInstance()
{
     static CreateWay *ptr = new CreateWay();
     return ptr;
}

Bot::CreateWay::WallsMap Bot::CreateWay::char2DArrToBlockMap(char **arrdat, int16_t _y, int16_t _x)
{
    Bot::CreateWay::WallsMap result;

    result.resize(_x);
    for (auto &i : result)
        i.resize(_y);

    for (int i = 0; i < _x; i++)
        for (int j = 0; j < _y; j++)
            result[i][j] = (arrdat[i][j] == '1' ? wall : nowall);

    return result;

}

std::vector<Bot::CreateWay::Int16Point>
Bot::CreateWay::createPointsPath(Bot::CreateWay::WallsMap &_map, Vec2 begin, Vec2 end)
{
    return createPointsPath(_map, Int16Point(begin.x, begin.y), Int16Point(end.x, end.y));
}

std::vector<Bot::CreateWay::Int16Point>
Bot::CreateWay::createPointsPath(WallsMap &_map, Int16Point begin, Int16Point end){

    // init map size
    mapx = _map.size();
    mapy = _map[0].size();

    blockMap = _map;

    // reset flags
    isCreated = false;
    cycle = false;

    // for reverse way - swap positions
    this->begin = end;
    this->end   = begin;

    // release points
    points.clear();
    outResult.clear();

    buildRoute();

    return outResult;

}
