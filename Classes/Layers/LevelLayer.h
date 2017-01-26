#ifndef __LEVELLAYER_H__
#define __LEVELLAYER_H__

#include "cocos2d.h"

class LevelLayer : public cocos2d::Sprite
{
public:
    enum {
        PhysicsBodyTag = 0x100
    };

public:
    virtual bool init();

    static LevelLayer* create(int level);

private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;

private:
    cocos2d::GLProgramState *glProgramState;
    float shaderSensitive;

protected:
    cocos2d::Sprite *image;
    cocos2d::Node *frame;

public:
    int levelIndex;
    int colorShader;

private:
    void initShader(const cocos2d::Vec2 &p1, const cocos2d::Vec2 &p2);
    void initLevel();
    void initFrame();
    void initPhysicsBodyLevel(int level); /* It is magic */

public:
    void setAttribShaderVec2(const cocos2d::Vec2 &p1, const cocos2d::Vec2 &p2);
    void setAttribMaskColor(float mask);
    void setAttribShaderSensitive(float sensitive);

    float getAttribMaskColor();

public:
    void shaderToSensitive(float time, float sensitive);

};

#endif // __LEVELLAYER_H__
