#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

static const std::string SoundEffectExplosion = "Sounds/Explosion.mp3";

class Audio : public cocos2d::Layer
{
private:
    Audio();

private:
    static CocosDenshion::SimpleAudioEngine *EngineInstance;
    static Audio *AudioInstance;
    unsigned int idEffect;

public:
    static Audio *getInstance();

    void preload();

    void playBackgroundMusic();
    void playBackgroundMusicMenu();
    void stopBackgroundMusic();
    void pauseBackgroundMusic();
    void resumeBackgroundMusic();

    void playSoundEffectExplosion();
    void stopEffectExplosion();
    void pauseEffectExplosion();
    void resumeEffectExplosion();

//    void playAllSounds();

};

#endif // __AUDIO_H__
