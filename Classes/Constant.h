#ifndef __CONSTANT_H__
#define __CONSTANT_H__

#include "cocos2d.h"

static const int LevelTimeGame[] = {
    4*60,
    5*60,
    6*60,
    7*60,
    8*60,
    8*60,
    9*60,
    10*60,
    10*60
};

static const char* Statuces[] = {
    "#First",

    "#The only thing in life achieved \n"
    "without effort is failure...", /* Единственное в жизни, что дается без усилий - это неудача... */

    "#Live each day as if your life \n"
    "had just begun...", /* Проживай каждый день, будто твоя жизнь только началась... */

    "#If you want to be somebody, \n"
    "somebody really special, \n"
    "be yourself...", /* Хочешь быть кем-то, кем-то действительно особенным - будь собой... */

    "#Genius is bort - not paid...", /* Гений должен быть рожден, а не проплачен... */

    "#Childhood sometimes does pay \n"
    "a second visit to man. \n"
    "Youth never..." /* Детство к человеку иногда возвращается. Юность никогда... */


};

static bool StartServer = false;

static float updateServer = 1.f / 60;

static const cocos2d::Size resolutionDisplay(1024, 600);

static const int discret = 15;

static const int countLevels = 10;

static const std::string NameBotOrOpponent = "BotOrOpponent";
static const std::string NamePlayer = "Player";
static const std::string NameParticle = "Particle";

// for ScoreLayer
static const int AlignLabel = 40;
static const int SizeFont = 24;
// end

static const float timeAnimationEat = 1.f / 5;

enum Levels {
    LSnake = 1,
    LLayer,
    LLight,
    LEat,
    LScore,
    LMenu,
    LParticle,
    LPause,
    LTop
};

enum GameMode {
    Survival,
    Classic,
    Local
};

enum GameType {
    LocalGame = 5,
    PlayerVSPlayer,
    PlayerVSBot,
    PlayerAlways,
    BotAlways
};

//GN == GameNavigator
enum Inits {
    InitNo                = 0,
    InitBot               = 1 << 0,
    InitFirstPlayer       = 1 << 1,
    InitSecondPlayer      = 1 << 2,
    InitGameNavigator     = 1 << 3,
    InitGameNavigatorAll  = 1 << 4,
    InitGNPlayer1Label    = 1 << 5,
    InitGNPlayer2Label    = 1 << 6,
    InitGNTimer           = 1 << 7,
    InitGNPauseButton     = 1 << 8,
    InitGNSoundMenu       = 1 << 9,
    InitEat               = 1 << 10,
    InitCamera            = 1 << 11,
    InitBonus             = 1 << 12,
    InitLocalPlayer       = 1 << 13,
    InitServer            = 1 << 14,
    InitAll               =~0
};

enum BitmaskCollision {
    BitEat    = 0b11,
    BitPlayer = 0b10,
    BitBot    = 0b01
};

enum TestBitmask {
    NoContact    = 0,
    SnakeBitMask = 1,
    EatBitMask   = ~0 /* all bit true */
};

enum Tags {
    TagPlayer = 0xA,
    TagBot,
    TagEat
};

static const int FPS = 60;
static const float updateTimeMSec = 1.f / FPS;
static const float SnakeMass = 0.001f;

#endif // __CONSTANT_H__
