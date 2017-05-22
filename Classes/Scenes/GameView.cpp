#include "GameView.h"
#include "Constant.h"
#include "MenuScene.h"
#include "GameOverScene.h"
#include "LevelPresentation.h"
#include "math/MathUtil.h"
#include "Sound/Audio.h"
#include "Data/DataSetting.h"
#include "Data/SendData.h"
#include <iostream>


#define PLAYER 0
#define PLAYER1 0
#define PLAYER2 1
#define BOT 1

#define NODE_TO_SNAKE(ptr) ((Snake*)((ptr)->getParent()))

#define ABS(value) ((value) < 0 ? -(value) : (value))

USING_NS_CC;

typedef GameOverLayer::ID_SNAKE ID_SNAKE;

inline Vec2 scaledVec2(const Vec2 &vec, const Vec2 &scl) {
    return Vec2(vec.x * scl.x, vec.y * scl.y);
}

GameView::~GameView()
{
    if (server) {
        delete server;
        server = nullptr;
    }

    localPlayer = nullptr;

}

Scene* GameView::createScene(int level, int bitmaskGame, int bitmaskGN, bool _showLearn)
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vec2::ZERO);
    scene->getPhysicsWorld()->setSubsteps(10);

    // 'Layer' is an autorelease object
    auto layer = GameView::create(level, bitmaskGame, bitmaskGN, _showLearn);
    layer->world = scene->getPhysicsWorld();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

GameView *GameView::create(int level, int bitmaskGame, int bitmaskGN, bool showLearn)
{
    GameView *pRet = new(std::nothrow) GameView();
    if (pRet) {
        pRet->levelIndex = level;
        pRet->bitmaskInitsGameLayer = bitmaskGame;
        pRet->bitmaskInitsGameNavigator = bitmaskGN;
        pRet->gameMode = GameData::mode;
        pRet->showLearn = showLearn;
    }
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}



// on "init" you need to initialize your instance
bool GameView::init()
{
    //////////////////////////////
    // 1. super init first
    if(!Layer::init())
        return false;

    log("initialize GameView...");

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    gameNavigatorLayer = nullptr;
    localPlayer = nullptr;
    playerActor = nullptr;
    botActor    = nullptr;
    eat         = nullptr;
    layer       = nullptr;
    bonus       = nullptr;
    server      = nullptr;

    isGameOver = false;

    snake[0] = snake[1] = nullptr;


    if (bitmaskInitsGameLayer & InitBot && bitmaskInitsGameLayer & InitSecondPlayer) {
        log("terminate. (InitBot && InitSecondPlayer) == true");
        std::terminate();
    }

    initCamera();

    initLayer();

    if (bitmaskInitsGameLayer & InitFirstPlayer) {

        initPlayers(playerActor, NamePlayer, UserData::playerColor);

        snake[PLAYER1] = playerActor;

        if (bitmaskInitsGameLayer & InitSecondPlayer)
            playerActor->setTouchRect(Rect(visibleSize.width/2, 0, visibleSize.width/2, visibleSize.height));
    }

    if (bitmaskInitsGameLayer & InitSecondPlayer) {

        initPlayers(player2Actor, NameBotOrOpponent, UserData::opponentColor);

        snake[PLAYER2] = player2Actor;

        player2Actor->setTouchRect(Rect(0, 0, visibleSize.width/2, visibleSize.height));

        player2Actor->keyLeft  = EventKeyboard::KeyCode::KEY_A;
        player2Actor->keyRight = EventKeyboard::KeyCode::KEY_D;
        player2Actor->keyUp    = EventKeyboard::KeyCode::KEY_W;
        player2Actor->keyDown  = EventKeyboard::KeyCode::KEY_S;
        player2Actor->keyStop  = EventKeyboard::KeyCode::KEY_X;

    }

    if (bitmaskInitsGameLayer & InitEat)
        initEat();

    if (bitmaskInitsGameLayer & InitGameNavigator)
    {
        log("init navigator");
        initGameNavigator();


    }

    if (bitmaskInitsGameLayer & InitBonus)
        initBonus();

    if (bitmaskInitsGameLayer & InitServer)
        initGameServer();

    if (bitmaskInitsGameLayer & InitLocalPlayer){
        initLocalPlayer();
        snake[PLAYER2] = localPlayer;
    }

    if (showLearn)
        showLearnControl();

    playingMusic();

    updateShaderPointsOfLevel();

    //////////////////
//    if (gameNavigatorLayer) {
//        gameNavigatorLayer->setTimeLevel(5);
//    }

//    if (playerActor) {
//        playerActor->addSnakeBlock(10);
//    }
    //////////////////
    return true;
}

void GameView::GoToGameView(int level, int bitmaskGame, int bitmaskGN, bool showLearn)
{
    Director::getInstance()->replaceScene(TransitionFade::create(1, GameView::createScene(level, bitmaskGame, bitmaskGN, showLearn)));
}

void GameView::initLayer()
{
    layer = LevelLayer::create(levelIndex);
    addChild(layer, LLayer);
}

void GameView::onEnterTransitionDidFinish()
{
    if (bitmaskInitsGameLayer & InitBot) {

        scheduleOnce ([this](float) {

            log("init bot tsans");

            initBotActor();

            if (botActor) {

                botActor->setWallsMap(getBlockMapLevel());
                botActor->startMovingHead();

                schedule([this](float){
                    botActor->setWallsMap(getBlockMapLevel());
                }, 1, "updmap");

                botActor->setGoTo(botActor->getPosition());

                snake[1] = botActor;

                if (snake[0])
                    snake[0]->opponent = snake[1];

                if (snake[1])
                    snake[1]->opponent = snake[0];
            }

        }, 0.5, "onceSetWallsMap");
    }
    else {
        if (snake[0])
            snake[0]->opponent = snake[1];

        if (snake[1])
            snake[1]->opponent = snake[0];
    }

    eat->setCallbackVerifyPosition(getVerifyPositionFunc());
    eat->setRandomPosition();

    layer->setCameraMask((unsigned short)CameraFlag::USER1);

    // test max speed for bot
//    scheduleOnce([this](float){ botActor->setSpeed(20); botActor->setEnableDebugDraw(true);}, 3, "stspd");

}

void GameView::initBonus()
{
    bonus = Bonus::create();

    if ((bitmaskInitsGameLayer & InitServer) || (!(bitmaskInitsGameLayer & InitServer) && !(bitmaskInitsGameLayer & InitLocalPlayer)))
        bonus->setMode(Bonus::Mode::Active);

    else bonus->setMode(Bonus::Mode::Passive);

    bonus->setCameraMask((unsigned int)CameraFlag::USER1);

    addChild(bonus, LEat);

    bonus->setCallbackContact(getCallbackContactFunctionBonus());

    bonus->setCallbackVerifyPosition(getVerifyPositionFunc());

    log ("hide position");

    bonus->hide();

}

void GameView::initEat()
{
    if (eat = NormalEat::create()) {

        if ((bitmaskInitsGameLayer & InitServer) || (!(bitmaskInitsGameLayer & InitServer) && !(bitmaskInitsGameLayer & InitLocalPlayer)))
            eat->setMode(Bonus::Mode::Active);

        else eat->setMode(Bonus::Mode::Passive);

        addChild(eat, Levels::LEat);
        eat->setCameraMask((unsigned int)CameraFlag::USER1);
        eat->setCallbackContact(getCallbackContactFunctionEat());
        eat->setCallbackVerifyPosition(getVerifyPositionFunc());
        eat->setPosition(Vec2(-200, -200));

        if ((bitmaskInitsGameLayer & InitBot) && !(bitmaskInitsGameLayer & InitFirstPlayer)) {

            auto listener = EventListenerTouchOneByOne::create();

            listener->onTouchBegan = [this](Touch *touch, Event *) -> bool {
                Vec2 offsetEatPos = camera->getPosition() - Vec2(visibleSize.width*camera->getAnchorPoint().x, visibleSize.height*camera->getAnchorPoint().y);
                eat->setPosition(touch->getLocation() + offsetEatPos);
                return true;
            };

            Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
        }

    }
}

void GameView::initBotActor()
{
    botActor = Bot::create();

    if (botActor) {

        botActor->setColor(UserData::opponentColor);

        botActor->setBonusPtr(bonus);
        botActor->setEatPtr(eat);

        addChild(botActor, Levels::LSnake);

        botActor->setCameraMask((unsigned int)CameraFlag::USER1);
        botActor->setName(NameBotOrOpponent);

        botActor->setSpeed(botActor->getSpeed()-1);

    }
}

void GameView::initLocalPlayer()
{
    localPlayer = LocalPlayer::create();
//    localPlayer->setView(this);

    localPlayer->setCallbackShotBomba([this](const Vec2 &in, const Vec2 &to) { shotTo(in, to); });

    localPlayer->setColor(UserData::opponentColor);
    localPlayer->setPosition({200, 200});
    localPlayer->setListen(playerActor);
    localPlayer->setSpeed(15);
    localPlayer->startMovingHead();

    addChild(localPlayer, LSnake);
    localPlayer->setName(NameBotOrOpponent);

    localPlayer->setEat(eat);
    localPlayer->setBonus(bonus);
    localPlayer->setLevelLayer(layer);
    localPlayer->setGNLayer(gameNavigatorLayer);
    localPlayer->setIsRunningServer(bitmaskInitsGameLayer & InitServer);

    schedule([this](float delay){
        localPlayer->getGameClient()->loop(delay);
    }, 0.01, "schLocPl");
}

void GameView::initPlayers(Player* &player, const std::string &name, const Color3B &color)
{
    player = Player::create();

    if (player) {

        player->setColor(color);

        addChild(player, Levels::LSnake);
        player->setCameraMask((unsigned short)CameraFlag::USER1);
        player->setName(name);

        player->setPosition({100, 300});

        player->startMovingHead();
    }
}

void GameView::initCamera()
{
    camera = Camera::createOrthographic(visibleSize.width, visibleSize.height, 1, 1024);
    camera->setContentSize(visibleSize);
    camera->setAnchorPoint(Vec2(0.5, 0.5));
    camera->setCameraFlag(CameraFlag::USER1);

    camera->setPosition(visibleSize / 2);
    camera->setPositionZ(1);

    addChild(camera, 1);

    schedule([=](float){

        static const float ContentScale = 1.f / Director::getInstance()->getContentScaleFactor();

        static const float maxx = visibleSize.width /2 + (resolutionDisplay.width *ContentScale - visibleSize.width);
        static const float maxy = visibleSize.height/2 + (resolutionDisplay.height*ContentScale - visibleSize.height);

        static const float minx = visibleSize.width  / 2;
        static const float miny = visibleSize.height / 2;

        if (!playerActor && !botActor)
            return;

        Vec2 pos;

        if (playerActor && player2Actor)
            pos = (playerActor->getPosition() + player2Actor->getPosition()) / 2;

        else if (playerActor)
            pos = playerActor->getPosition();

        else if (botActor)
            pos = botActor->getPosition();

        else return;

        float x = pos.x;
        float y = pos.y;

        if (y < miny) y = miny; else
        if (y > maxy) y = maxy;

        if (x < minx) x = minx; else
        if (x > maxx) x = maxx;

        x = (int)x;
        y = (int)y;

        // diapason (0, 0.05]
        static const float speedSetOffsetCamera = 0.05;

        // min offset
        static const float cameraSensitive = 0.01;

        auto cameraPosition = camera->getPosition();
        Vec2 camerraOffset = (Vec2(x, y) - cameraPosition) * speedSetOffsetCamera;

        if (ABS(camerraOffset.x) > cameraSensitive || ABS(camerraOffset.y) > cameraSensitive)
            camera->setPosition(cameraPosition + camerraOffset);

        if (!(bitmaskInitsGameLayer & InitCamera))
            unschedule("setcam");

    }, 1.f/60, "setcam");
}

void GameView::initGameServer()
{
    log("Init server");
    server = new GameServer(UserData::NetworkIp, UserData::NetworkPort);
    schedule([this](float delay){
        server->loop(delay);
    }, 0.01, "schServ");
}

void GameView::initGameNavigator()
{
    log("init navigator: %d", bitmaskInitsGameNavigator);

    gameNavigatorLayer = GameNavigatorLayer::create(gameMode, bitmaskInitsGameNavigator);

    addChild(gameNavigatorLayer, Levels::LScore);

    if (UserData::type == GameType::PlayerAlways)
        gameNavigatorLayer->setVisibleLabelOpponent(false);

    if (UserData::type == GameType::BotAlways)
        gameNavigatorLayer->setVisibleLabelPlayer(false);



    gameNavigatorLayer->setCallbackPause(getCallbackPause());
    gameNavigatorLayer->setCallbackTimeout(getCallbackTimeout());

    gameNavigatorLayer->setCallbackResume(getCalbackResume());
    gameNavigatorLayer->setCallbackRestart(getCallbackRestart());
    gameNavigatorLayer->setCallbackHome(getCallbackHome());

    gameNavigatorLayer->setCallbackButtonSoundMenu(getCallbackSoundButton());
    gameNavigatorLayer->setCallbackButtonEffect(getCallbackEffectButton());
    gameNavigatorLayer->setCallbackButtonMusic(getCallbackMusicButton());

    switch (gameMode) {

        case GameMode::Classic: {
            gameNavigatorLayer->setTimeLevel(LevelTimeGame[levelIndex-1]);
            gameNavigatorLayer->setStepSeconds(-1);
        } break;

        case GameMode::Survival: {
            gameNavigatorLayer->setTimeLevel(0);
            gameNavigatorLayer->setStepSeconds(1);
        } break;
    }


}

void GameView::showCoronaOnWinner()
{
    if (snake[0] && snake[1] && gameNavigatorLayer) {

        Snake  *objSnake = nullptr;

        if (UserData::type == LocalGame && bitmaskInitsGameLayer & InitServer) {
            objSnake = (gameNavigatorLayer->getScore(snake[0]->getName()) >
                        gameNavigatorLayer->getScore(snake[1]->getName()) ? snake[1] : snake[0]);
        }
        else {
            objSnake = (gameNavigatorLayer->getScore(snake[0]->getName()) >
                        gameNavigatorLayer->getScore(snake[1]->getName()) ? snake[0] : snake[1]);
        }

        Sprite *corona = Sprite::create("Corona.png");
        corona->setCameraMask((unsigned short)CameraFlag::USER1);
        corona->setPosition(objSnake->getPosition() + Vec2(0, objSnake->getCircleSize().height / 2));
        addChild(corona, LLight);

        Sprite *fadeCorona = Sprite::create("Corona.png");
        fadeCorona->setCameraMask((unsigned short)CameraFlag::USER1);
        fadeCorona->setPosition(corona->getPosition());
        addChild(fadeCorona, LLight);

        fadeCorona->runAction(Sequence::create(Spawn::create(ScaleBy::create(1, 2), FadeOut::create(1), nullptr),
                                               CallFunc::create([fadeCorona]{ fadeCorona->removeFromParent(); }),
                                               nullptr));
    }
}

void GameView::showLearnControl()
{
    Sprite *hand = Sprite::create("hand.png");
    float tenPercent = visibleSize.width * 0.10;

    hand->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);

    hand->setPosition(visibleSize.width/2 - tenPercent, visibleSize.height / 2);

    hand->setOpacity(0);

    addChild(hand, LTop + 1);

    scheduleOnce([=](float){

        auto *fadein = FadeIn::create(0.5);
          auto *inside  = MoveBy::create(0.3, Vec2(-tenPercent / 3, tenPercent / 3));
          auto *sclin   = ScaleTo::create(0.3, 0.9);
          auto *spawnin = Spawn::create(inside, sclin, nullptr);
        auto *move = MoveBy::create(2, Vec2(tenPercent * 2, 0));
          auto outside  = MoveBy::create(0.3, Vec2(tenPercent / 3, -tenPercent / 3));
          auto sclout   = ScaleTo::create(0.3, 1.0);
          auto spawnout = Spawn::create(outside, sclout, nullptr);
        auto *fadeout = FadeOut::create(0.5);

        const char *schname = "drawpoints";

        CallFunc *callfuncStart = CallFunc::create([=]{
            schedule([=](float){
                Sprite *ball = Sprite::create("circle.png");
                ball->setScale(0.10);
                ball->setPosition(hand->getPosition());
                addChild(ball, LTop);
                auto fade = FadeOut::create(1);
                auto remove = CallFunc::create([ball]{ ball->removeFromParent(); });
                auto seq = Sequence::create(fade, remove, nullptr);
                ball->runAction(seq);
            }, 0.1, schname);
        });

        CallFunc *callfuncStop = CallFunc::create([=]{
            unschedule(schname);
        });

        CallFunc *endCallback = CallFunc::create([hand]{
            hand->removeFromParent();
            UserData::isFirstPlaying = false;
            DataSetting::save();
        });

        Spawn *spawn = Spawn::create(move, callfuncStart, nullptr);

        Sequence *seq = Sequence::create(fadein, spawnin, spawn, callfuncStop, spawnout, fadeout, endCallback, nullptr);

        hand->runAction(seq);

    }, 2, "learn");

}

std::function<void (Node *node)> GameView::getCallbackContactFunctionEat() const
{
    return [this](Node *node) -> void {

        if (eat)
            eat->setRandomPosition();

        if ((snake[0] && node == snake[0]->head) || (snake[1] && node == snake[1]->head)){
            NODE_TO_SNAKE(node)->addSnakeBlock();

            log("getCallbackContactFunctionEat: %s", NODE_TO_SNAKE(node)->getName().c_str());

            if (node && gameNavigatorLayer)
                gameNavigatorLayer->addScores(1, node->getName());
        }

    };
}

std::function<void (Node *node)> GameView::getCallbackContactFunctionBonus() const
{
    return [this](Node *node) {

        log("EAT_BONUS");

        if(!(snake[0] && node == snake[0]->head) && !(snake[1] && node == snake[1]->head))
            return;

        NODE_TO_SNAKE(node)->eatBonus();

        switch (bonus->getBonusType()) {
        case Bonus::TypeBonusMask::BigScore:        eatBonusBigScore(node);      break;
        case Bonus::TypeBonusMask::FantazyShader:   eatBonusFantazyShader(node); break;
        case Bonus::TypeBonusMask::Bomba:           eatBonusBomba(node);         break;
        case Bonus::TypeBonusMask::LowSnake:        eatBonusSpeedMinus(node);    break;
        case Bonus::TypeBonusMask::FastSnake:       eatBonusSpeedPlus(node);     break;
        default: log("undefined bonus.");           std::terminate();
        };

    };
}

std::function<bool (const Vec2 &pos)> GameView::getVerifyPositionFunc() const
{
    return [this] (const Vec2 &pos) -> bool {

        //if distance between snakes heads < 100, then false
        for (const Snake *tmp : snake)
            if (tmp && tmp->getPosition().distance(pos) < 200)
                return false;

        //if distance between eat and bonus < 120, then false
        if (eat && bonus)
            if (eat->getPosition().distance(bonus->getPosition()) < 120)
                return false;

        // if physics world is not initialize, then true
        if (!world)
            return true;

        // if control point, in to place, then false
        for (const Vec2 &i : {Vec2(-discret, 0),
                              Vec2(0, discret),
                              Vec2(discret, 0),
                              Vec2(0, -discret)})
        {
            PhysicsShape *shape = world->getShape(pos + i);

            if (shape && shape->getBody()->getTag() == LevelLayer::PhysicsBodyTag)
                return false;
        }

        if (botActor)
            return botActor->createway(pos).size();

        return true;

    };
}

std::function<void ()> GameView::getCallbackPause() const
{
    return [this]() -> void {
        log("getCallbackPause");

        if (isGameOver)
            return;

        layer->setAttribShaderSensitive(1);

        if (gameNavigatorLayer) {
            gameNavigatorLayer->showPauseLayer();
            gameNavigatorLayer->setVisibleContentSoundMenu(true, true);
        }

        Director::getInstance()->pause();
        world->setSpeed(0);
        Audio::getInstance()->pauseEffectExplosion();

    };

}

std::function<void ()> GameView::getCalbackResume() const
{
    return [this]() -> void {

        layer->setAttribShaderSensitive(0);

        if (gameNavigatorLayer)
            gameNavigatorLayer->setVisibleContentSoundMenu(false, true);

        Director::getInstance()->resume();
        world->setSpeed(1);
        Audio::getInstance()->resumeEffectExplosion();
    };
}

std::function<void ()> GameView::getCallbackTimeout() const
{
    return [this]() -> void {

        if (!snake[0] || !snake[1])
            MenuScene::GoToMenuScene();

        else
            showGameOver();

    };
}

std::function<void ()> GameView::getCallbackRestart()
{
    return [this]() -> void {
        log("Restart");
//        if (server){
//            delete server;
//            server = nullptr;
//        }
        Director::getInstance()->resume();
        GameView::GoToGameView(levelIndex, bitmaskInitsGameLayer);
    };
}

std::function<void ()> GameView::getCallbackHome() const
{
    return [this]() -> void {
        log("Home");
        Director::getInstance()->resume();
        MenuScene::GoToMenuScene();
    };
}

std::function<void ()> GameView::getCallbackNext()
{
    return [this]() -> void {
        if (levelIndex + 1 <= countLevels)
            GameView::GoToGameView(levelIndex + 1, bitmaskInitsGameLayer, bitmaskInitsGameNavigator, UserData::isFirstPlaying);
    };
}

std::function<void ()> GameView::getCallbackSoundButton() const
{
    return [this] () -> void {
        log("GameView: getCallbackSoundButton()");
        if (Director::getInstance()->isPaused()){
            Director::getInstance()->resume();
            if (gameNavigatorLayer)
                gameNavigatorLayer->setVisibleContentSoundMenu(!gameNavigatorLayer->isVisibleSoundMenu(), true);
//            gameNavigatorLayer->update(updateTimeMSec);
//            gameNavigatorLayer->resumeSchedulerAndActions();
            Director::getInstance()->pause();
        }

    };
}

std::function<void ()> GameView::getCallbackMusicButton() const
{
    return [this] () -> void {
        if (UserData::playingBackgroundMusic)
            playingMusic();

        else Audio::getInstance()->pauseBackgroundMusic();
    };
}

std::function<void ()> GameView::getCallbackEffectButton() const
{
    log("getCallbackEffectButton");
    return [this] () -> void {
        if (!UserData::playingEffectSound)
            Audio::getInstance()->stopEffectExplosion();
    };
}

void GameView::playingMusic()
{
    if (UserData::playingBackgroundMusic && !Audio::getInstance()->isBackgroundMusicPlaying())
        Audio::getInstance()->playBackgroundMusic();
}

void GameView::stopingMusic()
{
    Audio::getInstance()->stopBackgroundMusic();
}

void GameView::eatBonusBigScore(Node *node)
{
    log("eatBonusBigScore");
    if (!node) return;

    if ((snake[0] && node == snake[0]->head) || (snake[1] && node == snake[1]->head)) {
        NODE_TO_SNAKE(node)->addSnakeBlock(5);

        if (gameNavigatorLayer)
            gameNavigatorLayer->addScores(5, node->getName().c_str());
    }
}

void GameView::eatBonusFantazyShader(Node *node)
{
    layer->shaderToSensitive(1, 1);

    scheduleOnce([this](float){

        int oldcolor = layer->getAttribMaskColor();
        int newcolor;

        do newcolor = cocos2d::random(1, 3); while(oldcolor == newcolor);

        layer->setAttribMaskColor(newcolor);
        layer->shaderToSensitive(1, 0);

    }, 1.1, "onc");
}

void GameView::eatBonusBomba(Node *node)
{
    if (!((snake[0] && node == snake[0]->head) || (snake[1] && node == snake[1]->head)))
        return;

    static const char *nameSch = "ShotBomba";

//    if (server && localPlayer)
//        localPlayer->setStatusShot({true, node->getName()});

    schedule([=](float){

        static const int countBullets = 12;

        static int value(countBullets);

        Vec2 in = node->getPosition();

        Vec2 to = NODE_TO_SNAKE(node)->opponent ? NODE_TO_SNAKE(node)->opponent->getPosition() : in;

        float dist = in.distance(to) / 5;

        shotTo(in, to + Vec2{cocos2d::random(-dist, dist), cocos2d::random(-dist, dist)});

        if (!--value) {
            value = countBullets;
            unschedule(nameSch);
        }

    }, 0.1, nameSch);

    scheduleOnce([=](float){
        if (UserData::playingEffectSound)
            Audio::getInstance()->playSoundEffectExplosion();
    }, 0.2, "playExplosion");
}

void GameView::eatBonusSpeedMinus(Node *node)
{
    log("sub speed");
    NODE_TO_SNAKE(node)->setSpeed(NODE_TO_SNAKE(node)->getSpeed() - 3);
}

void GameView::eatBonusSpeedPlus(Node *node)
{
    log("add speed");
    NODE_TO_SNAKE(node)->setSpeed(NODE_TO_SNAKE(node)->getSpeed() + 3);
}

void GameView::updateParticleShaderColors()
{
    if (layer) {
        switch ((int)layer->getAttribMaskColor()) {
        case 0: {
            particleExplosionColors.first = Color4B::RED;
            particleExplosionColors.second= Color4B::BLUE;
        } break;
        case 1: {
            particleExplosionColors.first = Color4B::BLUE;
            particleExplosionColors.second= Color4B::MAGENTA;
        } break;
        case 2: {
            particleExplosionColors.first = Color4B::GREEN;
            particleExplosionColors.second= Color4B::RED;
        } break;
        }
    }
}


void GameView::updateShaderPointsOfLevel()
{
    float scl = Director::getInstance()->getContentScaleFactor();

    float fx = visibleSize.width / resolutionDisplay.width;
    float fy = visibleSize.height / resolutionDisplay.height;

    Vec2 scaled(fx * scl, fy * scl);

    schedule([this, scaled](float){

        if (!snake[0] && !snake[1])
            layer->setAttribShaderVec2(visibleSize/2, visibleSize/2);

        else if (!snake[0] || !snake[1]) {
            Vec2 p = scaledVec2((snake[0] ? snake[0] : snake[1])->getPosition(), scaled);
            layer->setAttribShaderVec2(p, p);
        }

        else
            layer->setAttribShaderVec2(scaledVec2(snake[0]->getPosition(), scaled),
                                       scaledVec2(snake[1]->getPosition(), scaled));

    }, updateTimeMSec, "updateshaderattrib");
}

void GameView::gameover()
{
    GameOverScene::GoToGameOver();
}

void GameView::nextlevel()
{
    LevelPresentation::GoToLevelPresentation(levelIndex + 1, bitmaskInitsGameLayer);
    //    GameView::GoToGameView(levelIndex + 1);
}

void GameView::exploinsBomba(const Vec2 &posExplos)
{
    log("exploinsBomba");
    for (Snake* i : snake) {
        if (i && gameNavigatorLayer && i->isContainer(posExplos, 64) && gameNavigatorLayer->getScore(i->getName()) > 0) {
            gameNavigatorLayer->addScores(-1, i->getName());
            i->subSnake();
        }
    }
}

void GameView::shotTo(const Vec2 &in, const Vec2 &to)
{

//    updateParticleShaderColors();
    log("PrevCreateFile");
    auto fire = ParticleFire::createWithTotalParticles(1024);
    log("___");
    fire->setCameraMask((unsigned short)CameraFlag::USER1);
    fire->setEmitterMode(ParticleSystem::Mode::RADIUS);

    fire->setStartColor(Color4F::YELLOW);
    fire->setEndColor(Color4F::RED);

    fire->setStartRadius(3);
    fire->setLife(0.3);
    fire->setPosition(in);

    addChild(fire, LParticle);

    log("PrevCallback");

    auto callback = CallFunc::create([=](){

        log("scheduller");

        auto exp = ParticleSystemQuad::create("SunExplosion.plist");

//        updateParticleShaderColors();

        exp->setTotalParticles(512);
        exp->setEmitterMode(ParticleSystem::Mode::GRAVITY);
        exp->setCameraMask((unsigned short)CameraFlag::USER1);

        exp->setStartColor(Color4F::YELLOW);
        exp->setEndColor(Color4F::RED);

        exp->setScale(0.6 / _director->getContentScaleFactor());

        exp->setLife(.1);
        exp->setDuration(0.3);
        exp->setEndSpin(1);

        exp->setPosition(fire->getPosition());
        addChild(exp, LParticle);

        exploinsBomba(exp->getPosition());

        fire->removeFromParent();

    });

    auto moveto = MoveTo::create(0.5, to);
    auto fade = FadeOut::create(0.1);
    auto seq = Sequence::create(moveto, fade, callback, nullptr);
    fire->runAction(seq);
}

void GameView::setPositionEat(const Vec2 &pos)
{
    if (eat) {
        log("SET_POS_EAT!!!");
        eat->setPosition(pos);
    }
}

//void GameView::updateBot()
//{
//    if (botActor && eat) {
//        botActor->setGoTo(eat->getPosition());
//        botActor->updatePointsPath();
//    }
//}

cocos2d::Bot::CreateWay::WallsMap GameView::getBlockMapLevel()
{
    const float scaleFactor = _director->getContentScaleFactor();
    int sizex = resolutionDisplay.width / scaleFactor / discret;
    int sizey = visibleSize.height / scaleFactor / discret;

    char **walls = new char*[sizex];
    for (int i = 0; i < sizex; i++)
        walls[i] = new char[sizey];

    PhysicsShape *shape = nullptr;

    for (int i = 0; i < sizex; i++)
        for (int j = 0; j < sizey; j++)
            walls[i][j] = ((shape = world->getShape(Vec2(i*discret, j*discret))) && shape->getBody()->getTag() == LevelLayer::PhysicsBodyTag ? '1' : '0');

    auto result = Bot::CreateWay::char2DArrToBlockMap(walls, sizey, sizex);

    for(int i = 0; i < sizex; i++)
        delete []walls[i];

    delete []walls;

    return result;

}


void GameView::showGameOver()
{    
    isGameOver = true;

    for (Snake *objSnake : {snake[0], snake[1]})
        if (objSnake)
            objSnake->stopAll();

    if (bonus)
        bonus->shutdown();

    if (eat)
        eat->shutdown();

    gameNavigatorLayer->hideLabels(1);

    const int score_0 = gameNavigatorLayer->getScore(snake[0]->getName());
    const int score_1 = gameNavigatorLayer->getScore(snake[1]->getName());

    if (GOLayer = GameOverLayer::create(UserData::locale))
    {
        GOLayer->setCallbackHome(getCallbackHome());
        GOLayer->setCallbackRestart(getCallbackRestart());
        GOLayer->setCallbackNext(getCallbackNext());

        GOLayer->setSnakeName(ID_SNAKE::FIRST, UserData::playerName);
        GOLayer->setSnakeName(ID_SNAKE::SECOND,UserData::opponentName);

        for (ID_SNAKE id : {ID_SNAKE::FIRST, ID_SNAKE::SECOND}) {
            GOLayer->setScore(id, 0);
            GOLayer->setBonus(id, 0);
        }

        GOLayer->setPosition(visibleSize / 2);
        addChild(GOLayer, LTop + 1);

//        GOLayer->setScore(ID_SNAKE::FIRST, gameNavigatorLayer->getScore(playerActor->getName()));

        if (score_0 == score_1) {
            GOLayer->showDanceWin(ID_SNAKE::NO_WINS);
        }
        else if (UserData::type == LocalGame && bitmaskInitsGameLayer & InitServer) {
            GOLayer->showDanceWin(score_0 > score_1 ? ID_SNAKE::SECOND : ID_SNAKE::FIRST);
            GOLayer->showFireworks();
        }
        else {
            GOLayer->showDanceWin(score_0 > score_1 ? ID_SNAKE::FIRST : ID_SNAKE::SECOND);
            GOLayer->showFireworks();
        }
    }

    if (score_0 != score_1)
        showCoronaOnWinner();

    layer->shaderToSensitive(5, 0.75);

    const char *schRemoveSnakeBlocks = "schremblcks";

    schedule([=](float){

        bool run = false;

        for (auto obj : {std::make_pair(snake[0], ID_SNAKE::FIRST),
                         std::make_pair(snake[1], ID_SNAKE::SECOND)})
        {
            Snake *snk = obj.first;
            ID_SNAKE id = obj.second;

            int score = gameNavigatorLayer->getScore(snk->getName());
            if (GOLayer->getScore(id) < score) {
                run = true;
                GOLayer->setScore(id, GOLayer->getScore(id) + 1);
            }

            int bonusses = snk->getCountBonusses();

            if (GOLayer->getBonus(id) < bonusses) {
                run = true;
                GOLayer->setBonus(id, GOLayer->getBonus(id) + 1);
            }
        }

        if (!run)
            unschedule(schRemoveSnakeBlocks);

    }, 0.1, schRemoveSnakeBlocks);
}
