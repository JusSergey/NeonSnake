#include "Audio.h"

USING_NS_CC;


/* Begin init Audio::EngineINstance, and last Audio::AudioInstance! */

static const int countMusics = 2;
static const std::string prename = "Sounds/Background_";

using StringUtils::toString;

CocosDenshion::SimpleAudioEngine* Audio::EngineInstance = nullptr;//CocosDenshion::SimpleAudioEngine::getInstance(); crash on android

Audio* Audio::AudioInstance = new Audio;


Audio::Audio() : idEffect(0) { }

Audio *Audio::getInstance()
{
    if (Audio::EngineInstance == nullptr)
        EngineInstance = CocosDenshion::SimpleAudioEngine::getInstance();

//    if (Audio::EngineInstance == nullptr)
//        Audio::AudioInstance = new Audio;

    return Audio::AudioInstance;
}

void Audio::preload()
{
    Audio::EngineInstance->preloadEffect(SoundEffectExplosion.c_str());

    for (int i = 0; i < countMusics; i++)
        Audio::EngineInstance->preloadBackgroundMusic((prename + toString(i) + ".mp3").c_str());
}

void Audio::playSoundEffectExplosion()
{
    if (idEffect)
        Audio::EngineInstance->stopEffect(idEffect);

    idEffect = Audio::EngineInstance->playEffect(SoundEffectExplosion.c_str());
}

void Audio::playBackgroundMusic()
{
    Audio::EngineInstance->stopBackgroundMusic();
    Audio::EngineInstance->playBackgroundMusic((prename + "1.mp3").c_str(), true);
}

void Audio::playBackgroundMusicMenu()
{
    Audio::EngineInstance->stopBackgroundMusic();
    Audio::EngineInstance->playBackgroundMusic((prename + "0.mp3").c_str(), true);
}

void Audio::pauseBackgroundMusic()
{
    Audio::EngineInstance->pauseBackgroundMusic();
}

void Audio::resumeBackgroundMusic()
{
    Audio::EngineInstance->resumeBackgroundMusic();
}

void Audio::stopBackgroundMusic()
{
    Audio::EngineInstance->stopBackgroundMusic();
}

void Audio::stopEffectExplosion()
{
    Audio::EngineInstance->stopEffect(idEffect);
}

void Audio::pauseEffectExplosion()
{

    Audio::EngineInstance->pauseEffect(idEffect);
}

void Audio::resumeEffectExplosion()
{
    Audio::EngineInstance->resumeEffect(idEffect);
}
