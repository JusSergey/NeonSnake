#ifndef __BOT_H__
#define __BOT_H__

#include "cocos2d.h"
#include "Snake.h"
#include "Constant.h"
#include "Actions/Bonus.h"
#include "Actions/NormalEat.h"

NS_CC_BEGIN

class Bot : public cocos2d::Snake
{
public:
    virtual bool init();

private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    int indexPoint;
    Vec2 GoTo;

public:
    inline void setGoTo(const Vec2 &to) { GoTo = to; }

public:
    inline bool isCreateWay() const { return pointsPath.size(); }

protected:
    virtual void movingHead(float delta);

////////////////////// AI CREATE PATH
public:
    class CreateWay {
    public:

        struct Int16Point {
            Int16Point(){}
            Int16Point(int16_t _x, int16_t _y){
                x = _x;
                y = _y;
            }

            int16_t x;
            int16_t y;
            Vec2 toVec2() const {
                return Vec2(x, y);
            }

            bool operator == (const Int16Point &obj) {
                return (obj.x == x && obj.y == y);
            }
            bool operator != (const Int16Point &obj) {
                return (obj.x != x || obj.y != y);
            }
        };

        bool isNoWalls(Int16Point p);

        struct Wall {
            Wall () {}
            Wall (char _sym) : symbol(_sym) {}
            char symbol;
            Int16Point parent;
        };

    public:
        typedef std::vector<std::vector<Wall>> WallsMap;

    private:
        std::vector<Int16Point> points;
        WallsMap blockMap;
        int_fast16_t mapx;
        int_fast16_t mapy;
        Int16Point begin;
        Int16Point end;
        bool isCreated;
        bool cycle;

    public:
        static CreateWay *getInstance();
        static WallsMap char2DArrToBlockMap(char **arrdat, int16_t _y, int16_t _x);
        std::vector<Int16Point> createPointsPath(WallsMap &_map, Int16Point begin, Int16Point end);
        std::vector<Int16Point> createPointsPath(WallsMap &_map, Vec2 begin, Vec2 end);

    private:
        void parrent();
        void addRect();
        void control();
        void buildRoute();

    private:
        std::vector<Int16Point> outResult;
    };

public:
    std::vector<CreateWay::Int16Point> createway(const Vec2 &in, const Vec2 &to);
    std::vector<CreateWay::Int16Point> createway(const Vec2 &to);

private:
    CreateWay::WallsMap walls;
    std::vector<CreateWay::Int16Point> pointsPath;

    ////////////////////// AI BOT
private:
    // in analise()
    struct {
        BasicEat::Tags tagObject = (BasicEat::Tags)-1;
        bool visibleBonus = false;
        cocos2d::Vec2 prevPosBonus;
        cocos2d::Vec2 prevPosEat;
    } analizeData;

    void analizeSituation(float = 0);

    cocos2d::Bonus *bonus;
    NormalEat      *eat;

public:
    inline void setBonusPtr(cocos2d::Bonus *ptr) { bonus = ptr; }
    inline void setEatPtr(NormalEat* ptr) { eat = ptr; }

public:
    static Bot* create(CreateWay::WallsMap bMap/* = CreateWay::WallsMap()*/)
    {
        Bot *pRet = new(std::nothrow) Bot();
        if (pRet && pRet->init())
        {
            pRet->autorelease();
            pRet->walls = bMap;
            return pRet;
        }
        else
        {
            delete pRet;
            pRet = nullptr;
            return nullptr;
        }
    }

    CREATE_FUNC(Bot)

    inline void setWallsMap(const CreateWay::WallsMap &WMap) { walls = WMap; }

};

NS_CC_END

#endif // __BOT_H__
