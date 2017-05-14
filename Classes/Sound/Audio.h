#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

//enum Part {
//    _1 = 1, _2, _3
//};

class Audio
{
private:
    Audio();

private:
    static CocosDenshion::SimpleAudioEngine *EngineInstance;
    static Audio *AudioInstance;
    unsigned int idEffect;
//    unsigned int idPart;

public:
    static Audio *getInstance();

    void preload();

    void playBackgroundMusic();
    void stopBackgroundMusic();
    void pauseBackgroundMusic();
    void resumeBackgroundMusic();
    void playSoundEffectExplosion();
    void stopEffectExplosion();
    void pauseEffectExplosion();
    void resumeEffectExplosion();

    bool isBackgroundMusicPlaying() { return EngineInstance->isBackgroundMusicPlaying(); }
};

#endif // __AUDIO_H__
