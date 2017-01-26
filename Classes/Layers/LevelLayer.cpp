#include "LevelLayer.h"
#include "../MyBodyParser.h"
#include "Constant.h"

USING_NS_CC;


using StringUtils::toString;

Sprite* getSpriteLevel(int level) {
    return Sprite::create("Levels/level_" + toString(level) + ".png");
}

void LevelLayer::initPhysicsBodyLevel(int level) {

    Sprite *sprite = Sprite::create();
    sprite->setContentSize(resolutionDisplay);
    sprite->setPosition(visibleSize / 2);

    MyBodyParser::getInstance()->parseJsonFile("Levels/level_" + toString(level) + ".json");

    sprite->setPhysicsBody(MyBodyParser::getInstance()->bodyFormJson(this, "body", PhysicsMaterial()));
    sprite->getPhysicsBody()->setDynamic(false);
    sprite->getPhysicsBody()->setTag(PhysicsBodyTag);

    sprite->setOpacity(0);

    addChild(sprite);

}

// on "init" you need to initialize your instance
bool LevelLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if(!Sprite::init())
        return false;

    log("initialize LevelLayer...");

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    image = getSpriteLevel(levelIndex);
    setTexture(image->getTexture());
    setColor(Color3B::BLACK);
    setContentSize(image->getContentSize());
    image->setAnchorPoint(Vec2::ZERO);
    addChild(image, LLayer);

    setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

    image->setAnchorPoint(Vec2::ZERO);

    initPhysicsBodyLevel(levelIndex);

    setPosition(Vec2::ZERO);

    initShader(Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);


    return true;
}

LevelLayer *LevelLayer::create(int level)
{
    LevelLayer *pRet = new(std::nothrow) LevelLayer();
    if (pRet) pRet->levelIndex = level;
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

void LevelLayer::initShader(const Vec2 &p1, const Vec2 &p2)
{
    const GLchar *fragmentShader = "MyShaders/light.fsh";
    const GLchar *vertexShader   = "MyShaders/light.vsh";

    GLProgram *glProgramLevel = new GLProgram();
    glProgramLevel->initWithFilenames(vertexShader, fragmentShader);
    glProgramLevel->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION,  GLProgram::VERTEX_ATTRIB_POSITION);
    glProgramLevel->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR,     GLProgram::VERTEX_ATTRIB_COLOR);
    glProgramLevel->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
    glProgramLevel->link();

    glProgramLevel->updateUniforms();

    image->setShaderProgram(glProgramLevel);

    glProgramState = image->getGLProgramState();

    glProgramState->setUniformFloat("blackwhite", shaderSensitive = .0f);
    glProgramState->setUniformVec3("filter", Vec3(1, 0, 1));
    glProgramState->setUniformTexture("u_texture", image->getTexture());
    glProgramState->setUniformVec2("u_center1", p1);
    glProgramState->setUniformVec2("u_center2", p2);
    glProgramState->setUniformFloat("maskShader", colorShader = cocos2d::random(0, 3));
    glProgramLevel->use();


}

void LevelLayer::initLevel()
{

    image = getSpriteLevel(levelIndex);
    image->setAnchorPoint(Vec2(0.5, 0.5));
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    image->setPosition(visibleSize / 2);

    image->setPhysicsBody(PhysicsBody::createBox(Size(50, 50)));

    addChild(image);

    image->getPhysicsBody()->setDynamic(false);

}

void LevelLayer::initFrame()
{
    frame = Node::create();
    frame->setPosition(visibleSize / 2);
    frame->setPhysicsBody(PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 5));
    addChild(frame);
}

void LevelLayer::setAttribShaderVec2(const Vec2 &p1, const Vec2 &p2)
{
    Vec2 correctPoint1(p1.x / visibleSize.width, 1.f - p1.y / visibleSize.height);
    Vec2 correctPoint2(p2.x / visibleSize.width, 1.f - p2.y / visibleSize.height);

    glProgramState->setUniformVec2("u_center1", correctPoint1);
    glProgramState->setUniformVec2("u_center2", correctPoint2);
    glProgramState->setUniformTexture("u_texture", image->getTexture());
}

void LevelLayer::setAttribMaskColor(float mask)
{
    colorShader = mask;
    glProgramState->setUniformFloat("maskShader", mask);
}

void LevelLayer::setAttribShaderSensitive(float sensitive)
{
    glProgramState->setUniformFloat("blackwhite", sensitive);
}

float LevelLayer::getAttribMaskColor()
{
    return colorShader;
}

float abs(float a){
    return (a < 0 ? -a : a);
}

void LevelLayer::shaderToSensitive(float time, float sensitive)
{
    static const char *nameScheduler = "NameSchSenShader";

    float step = (sensitive - shaderSensitive) * updateTimeMSec;

    bool is = sensitive > shaderSensitive;

    log("into[%f, %f], step[%f]", shaderSensitive, sensitive, step);

    schedule([=](float){

        if (sensitive > shaderSensitive != is)
            unschedule(nameScheduler);

        setAttribShaderSensitive(shaderSensitive += step);

    }, updateTimeMSec, nameScheduler);

}
