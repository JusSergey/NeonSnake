#ifndef __GAMEVIEW_H__
#define __GAMEVIEW_H__

#include "cocos2d.h"
#include "Layers/LevelLayer.h"
#include "Actors/LocalPlayer.h"
#include "Actors/Player.h"
#include "Actors/Bot.h"
#include "Actions/NormalEat.h"
#include "Actions/Bonus.h"
#include "Layers/GameNavigatorLayer.h"
#include "Layers/GameOverLayer.h"
#include "Network/GameServer.h"
#include "Constant.h"

class GameView : public cocos2d::Layer
{

public:
    virtual ~GameView();

    static cocos2d::Scene* createScene(int level, int bitmaskGame, int bitmaskGN, bool _showLearn = false);

    virtual bool init();

    static void GoToGameView(int level, int bitmaskGame = InitAll, int bitmaskGN = InitGameNavigatorAll, bool showLearn = false);

    static GameView *create(int level, int bitmaskGame, int bitmaskGN, bool showLearn);

protected:
    bool showLearn;
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::PhysicsWorld *world;

protected:
    std::pair<cocos2d::Color4B, cocos2d::Color4B> particleExplosionColors;
    cocos2d::LocalPlayer *localPlayer;
    cocos2d::Player *player2Actor;
    cocos2d::Player *playerActor;
    cocos2d::Bot    *botActor;

    cocos2d::Snake* snake[2];

    GameNavigatorLayer *gameNavigatorLayer;
    GameOverLayer   *GOLayer;
    NormalEat       *eat;
    cocos2d::Bonus  *bonus;
    cocos2d::Camera *camera;
    GameServer      *server;

protected:
    int bitmaskInitsGameLayer;
    int bitmaskInitsGameNavigator;
    GameMode gameMode;
    int levelIndex;
    bool isGameOver;

protected:
    LevelLayer *layer;

protected:
    virtual void onEnterTransitionDidFinish();

private:
    void initLayer      ();
    void initBonus      ();
    void initEat        ();
    void initBotActor   ();
    void initLocalPlayer();
    void initCamera     ();
    void initGameServer ();
    void initGameNavigator();

    void showCoronaOnWinner();
    void showLearnControl();
    void showGameOver();

    void initPlayers    (cocos2d::Player*       &player,
                         const std::string      &name,
                         const cocos2d::Color3B &color);

protected:
    virtual std::function<void(Node *node)> getCallbackContactFunctionEat()   const;
    virtual std::function<void(Node *node)> getCallbackContactFunctionBonus() const;

    virtual std::function<bool(const cocos2d::Vec2 &pos)> getVerifyPositionFunc() const;

    virtual std::function<void()> getCallbackPause()    const;
    virtual std::function<void()> getCalbackResume()    const;
    virtual std::function<void()> getCallbackTimeout()  const;
    virtual std::function<void()> getCallbackRestart()  ;
    virtual std::function<void()> getCallbackHome()     const;
    virtual std::function<void()> getCallbackNext()     ;
    virtual std::function<void()> getCallbackSoundButton()  const;
    virtual std::function<void()> getCallbackMusicButton()  const;
    virtual std::function<void()> getCallbackEffectButton() const;

    virtual void playingMusic();
    virtual void stopingMusic();

private:
    void eatBonusBigScore       (cocos2d::Node *node);
    void eatBonusFantazyShader  (cocos2d::Node *node);
    void eatBonusBomba          (cocos2d::Node *node);
    void eatBonusSpeedMinus     (cocos2d::Node *node);
    void eatBonusSpeedPlus      (cocos2d::Node *node);

    void updateParticleShaderColors();
    void updateShaderPointsOfLevel();
    void gameover();
    void nextlevel();
    void exploinsBomba(const cocos2d::Vec2 &posExplos);

public:
    void shotTo(const cocos2d::Vec2 &in, const cocos2d::Vec2 &to);

public:
    void setPositionEat(const cocos2d::Vec2 &pos);
//    void updateBot();

public:
    cocos2d::Bot::CreateWay::WallsMap getBlockMapLevel();

};

#endif // __GAMEVIEW_H__
