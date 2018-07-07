#include "AppDelegate.h"
//#include "GameScene.h"
#include "MenuScene.h"

// #define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

// 设置游戏窗口大小为1280*720
static cocos2d::Size designResolutionSize = cocos2d::Size(1280, 720);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
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
        glview = GLViewImpl::createWithRect("Kingdom", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("Kingdom");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    auto frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }

    register_all_packages();

	// 加载游戏资源
	// 添加enemy1帧动画
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy1.plist");
	char totalFrames_enemy1 = 11;
	char frameName_enemy1[40];
	Animation* enemy1Animation = Animation::create();
	for (int i = 0; i < totalFrames_enemy1; i++) {
		sprintf(frameName_enemy1, "enemy1_%d.png", i);
		enemy1Animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName_enemy1));
	}
	enemy1Animation->setDelayPerUnit(0.1);
	AnimationCache::getInstance()->addAnimation(enemy1Animation, "enemy1Animation");

	totalFrames_enemy1 = 7;
	Animation* enemy1AttackAnimation = Animation::create();
	for (int i = 0; i < totalFrames_enemy1; i++) {
		sprintf(frameName_enemy1, "enemy1_attack_%d.png", i);
		enemy1AttackAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName_enemy1));
	}
	enemy1AttackAnimation->setDelayPerUnit(0.1);
	AnimationCache::getInstance()->addAnimation(enemy1AttackAnimation, "enemy1AttackAnimation");

	totalFrames_enemy1 = 5;
	Animation* enemy1DeadAnimation = Animation::create();
	for (int i = 0; i < totalFrames_enemy1; i++) {
		sprintf(frameName_enemy1, "enemy1_dead_%d.png", i);
		enemy1DeadAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName_enemy1));
	}
	enemy1DeadAnimation->setDelayPerUnit(0.1);
	AnimationCache::getInstance()->addAnimation(enemy1DeadAnimation, "enemy1DeadAnimation");


    // create a scene. it's an autorelease object
    auto scene = MenuScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
