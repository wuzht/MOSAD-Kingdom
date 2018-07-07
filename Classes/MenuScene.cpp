#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"

USING_NS_CC;

Scene* MenuScene::createScene()
{
    return MenuScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// 背景音乐
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("sounds/Jungle_Prep.mp3");
	audio->playBackgroundMusic("sounds/Jungle_Prep.mp3", true);
	
	// 开始场景的背景图片
	auto startBg = Sprite::create("StartBg.png");
	startBg->setScale(0.667);
	startBg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(startBg, 0);

	// 选择难度栏的木板
	auto optionsBoard = Sprite::create("OptionsBoard.png");
	optionsBoard->setScale(0.667);
	optionsBoard->setPosition(Vec2(visibleSize.width / 2 + origin.x - 400, visibleSize.height / 2 + origin.y - 80));
	this->addChild(optionsBoard, 1);

	auto options = Sprite::create("Options.png");
	options->setRotation(2);
	options->setScale(1.1);
	options->setPosition(Vec2(visibleSize.width / 2 + origin.x - 360, visibleSize.height / 2 + origin.y + 50));
	this->addChild(options, 2);
	
	// 难度1
	auto choice1 = MenuItemImage::create(
		"OneNormal.png",
		"OneSelected.png",
		CC_CALLBACK_1(MenuScene::startCallback, this)
	);
	choice1->setScale(1.2);
	choice1->setPosition(Vec2(visibleSize.width / 2 + origin.x - 360, visibleSize.height / 2 + origin.y - 42));

	auto menu1 = Menu::create(choice1, NULL);
	menu1->setPosition(Vec2::ZERO);
	this->addChild(menu1, 3);

	// 难度2
	auto choice2 = MenuItemImage::create(
		"TwoNormal.png",
		"TwoSelected.png",
		CC_CALLBACK_1(MenuScene::startCallback, this)
	);
	choice2->setScale(1.2);
	choice2->setPosition(Vec2(visibleSize.width / 2 + origin.x - 345, visibleSize.height / 2 + origin.y - 105));

	auto menu2 = Menu::create(choice2, NULL);
	menu2->setPosition(Vec2::ZERO);
	this->addChild(menu2, 3);

	// 难度3
	auto choice3 = MenuItemImage::create(
		"ThreeNormal.png",
		"ThreeSelected.png",
		CC_CALLBACK_1(MenuScene::startCallback, this)
	);
	choice3->setScale(1.2);
	choice3->setPosition(Vec2(visibleSize.width / 2 + origin.x - 365, visibleSize.height / 2 + origin.y - 162));

	auto menu3 = Menu::create(choice3, NULL);
	menu3->setPosition(Vec2::ZERO);
	this->addChild(menu3, 3);

    return true;
}

// 选择难度后的回调函数，跳转到游戏场景GameScene
void MenuScene::startCallback(Ref* pSender) {
	auto newScene = GameSence::createScene();
	Director::getInstance()->replaceScene(CCTransitionFade::create(0.5, newScene));
}

