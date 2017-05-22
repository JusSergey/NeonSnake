#include "AppDelegate.h"
#include "Data/DataSetting.h"
#include "Scenes/TitleScene.h"
#include "Constant.h"

USING_NS_CC;

#define TRUE  1
#define FALSE 0
//
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
# define isFullScreen TRUE
#else
# define isFullScreen TRUE
#endif

#if isFullScreen
static cocos2d::Size dsz = Director::getInstance()->getVisibleSize();
#else
static cocos2d::Size dsz(1024, 600);
#endif


static cocos2d::Size designResolutionSize = cocos2d::Size(dsz);
static cocos2d::Size smallResolutionSize  = cocos2d::Size(dsz);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(dsz);//(1024, 768);
static cocos2d::Size largeResolutionSize  = cocos2d::Size(dsz);//(2048, 1536);
AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {

    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

  #if isFullScreen
        glview = GLViewImpl::createWithFullScreen("neonsnake");
  #else
       glview = GLViewImpl::createWithRect("Neon snake", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
  #endif

#else
        glview = GLViewImpl::create("Neon Snake");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);
    
//    Socket::startThread();

//    unsigned short basePort = 2121;

//    for (auto msg : {"Thread 1. My name", "Thread 2. Juice", "Thread 3. Linux"}){
//        Socket *s = new Socket("127.0.0.1", ++basePort, "127.0.0.1", ++basePort);
////        s->setMsg(msg);
////        s->loopSendMessage();
////        s->loopRecvMessage();
//    }

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(updateTimeMSec);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);


    auto visibleSize = Director::getInstance()->getVisibleSize();

    float ScaleFactorX = visibleSize.width / resolutionDisplay.width;
    float ScaleFactorY = visibleSize.height/ resolutionDisplay.height;

    if (ScaleFactorX != 1 || ScaleFactorY != 1) {
        log("Scale Factor xy[%f, %f]", ScaleFactorX, ScaleFactorY);
        Director::getInstance()->setContentScaleFactor(1.f / std::max(ScaleFactorX, ScaleFactorY));
    }

    FileUtils::getInstance()->removeFile(ServerLogFilePath);
    FileUtils::getInstance()->removeFile(ClientLogFilePath);

    register_all_packages();

    // load user data
    DataSetting::load();

    // create a scene. it's an autorelease object
    auto scene = TitleScene::createScene();


    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
