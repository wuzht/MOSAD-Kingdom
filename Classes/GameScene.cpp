#include "GameScene.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

// 因为图片素材是用在1920*1080分辨率窗口里的，而本游戏窗口大小为1280*920，故要设置scale
#define MY_SCALE_SIZE 0.54

Scene* GameSence::createScene()
{
	auto scene = Scene::create();
	auto layer = GameSence::create();
	scene->addChild(layer);
	return scene;
}

// on "init" you need to initialize your instance
bool GameSence::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	float myXPosition = visibleSize.width / 2 + origin.x;
	float myYPosition = visibleSize.height / 2 + origin.y;

	// 背景音乐
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("sounds/Jungle_Battle.mp3");
	audio->playBackgroundMusic("sounds/Jungle_Battle.mp3", true);

	// 背景图片
	auto gameBg = Sprite::create("GameBg.png");
	gameBg->setScale(MY_SCALE_SIZE);
	gameBg->setPosition(Vec2(myXPosition, myYPosition));
	this->addChild(gameBg, 0);

	// 血条后面那个框框
	auto healthBarContaier = Sprite::create("HealthBar1.png");
	healthBarContaier->setScale(1, 0.5);
	healthBarContaier->setPosition(Vec2(myXPosition, myYPosition + 320));
	this->addChild(healthBarContaier, 10);

	// 血条（图腾的血量，图腾血量为0就游戏结束）
	healthBar = Sprite::create("HealthBar2.png");
	healthBar->setScale(1, 0.5);
	healthBar->setPosition(Vec2(myXPosition, myYPosition + 313));
	this->addChild(healthBar, 9);

	// 图腾
	auto totem = Sprite::create("Totem.png");
	totem->setScale(MY_SCALE_SIZE);
	totem->setPosition(Vec2(myXPosition, myYPosition - 300));
	this->addChild(totem, 2);

	// 图腾子弹
	auto totemBullet = Sprite::create("TotemBullet.png");
	totemBullet->setScale(MY_SCALE_SIZE);
	// 这是图腾子弹射击的起始位置
	totemBullet->setPosition(Vec2(myXPosition + 16, myYPosition - 258));
	this->addChild(totemBullet, 3);

	// 塔位数量
	towerLandsNum = 12;

	// 各个塔位的位置
	// 每个绿色地为一块，下面的塔位位置的顺序是从左到右
	// 左上块
	towerLandsPositions.push_back(Vec2(myXPosition - 530, myYPosition + 165));	// 0
	towerLandsPositions.push_back(Vec2(myXPosition - 210, myYPosition + 150));	// 1
	towerLandsPositions.push_back(Vec2(myXPosition- 10, myYPosition + 230));	// 2

	towerLandsPositions.push_back(Vec2(myXPosition + 55, myYPosition + 20));	// 3
	towerLandsPositions.push_back(Vec2(myXPosition + 180, myYPosition - 100));	// 4

	// 左块
	towerLandsPositions.push_back(Vec2(myXPosition - 260, myYPosition + 10));	// 5

	// 左下块
	towerLandsPositions.push_back(Vec2(myXPosition - 320, myYPosition - 120));	// 6
	towerLandsPositions.push_back(Vec2(myXPosition - 100, myYPosition - 100));	// 7
	towerLandsPositions.push_back(Vec2(myXPosition - 30, myYPosition - 150));	// 8

	// 右上块
	towerLandsPositions.push_back(Vec2(myXPosition + 180, myYPosition + 180));	// 9
	towerLandsPositions.push_back(Vec2(myXPosition + 310, myYPosition + 60));	// 10

	// 右下块
	towerLandsPositions.push_back(Vec2(myXPosition + 390, myYPosition - 95));	// 11

	// 放置塔位
	for (int i = 0; i < towerLandsNum; i++)
	{
		auto towerLand = Sprite::create("TowerLand.png");
		towerLand->setScale(MY_SCALE_SIZE);
		towerLand->setPosition(towerLandsPositions[i]);
		// 放到towerLands里，以便后面处理，可根据需要修改
		towerLands.push_back(towerLand);
		this->addChild(towerLand, 1);
	}

	// 塔和对应的子弹，注意各个塔的scale值和位置偏移量如下设置，不要设置其他值（示例）
	auto tower1 = Sprite::create("tower1.png");
	tower1->setScale(0.7);
	// 各个塔位的位置的索引见上面的注释。塔的位置要加上一个偏移量如Vec2(-2, 20)。
	tower1->setPosition(towerLandsPositions[4] + Vec2(-2, 16));
	this->addChild(tower1, 2);
	// 子弹
	auto tower1Bullet = Sprite::create("tower1Bullet.png");
	tower1Bullet->setScale(0.8);
	tower1Bullet->setPosition(towerLandsPositions[4] + Vec2(-2, 50));
	this->addChild(tower1Bullet, 3);

	auto tower2 = Sprite::create("tower2.png");
	tower2->setScale(0.8);
	tower2->setPosition(towerLandsPositions[8] + Vec2(-4, 20));
	this->addChild(tower2, 2);
	// 子弹
	auto tower2Bullet = Sprite::create("tower2Bullet.png");
	tower2Bullet->setScale(0.5);
	tower2Bullet->setPosition(towerLandsPositions[8] + Vec2(-4, 60));
	this->addChild(tower2Bullet, 3);

	auto tower3 = Sprite::create("tower3.png");
	tower3->setScale(0.8);
	tower3->setPosition(towerLandsPositions[3] + Vec2(0, 4));
	this->addChild(tower3, 2);
	// 子弹
	auto tower3Bullet = Sprite::create("tower3Bullet.png");
	tower3Bullet->setScale(0.8);
	tower3Bullet->setPosition(towerLandsPositions[3] + Vec2(-4, 20));
	this->addChild(tower3Bullet, 3);

	// 返回主菜单按钮
	auto quitButton = MenuItemImage::create(
		"QuitNormal.png",
		"QuitSelected.png",
		CC_CALLBACK_1(GameSence::quitCallback, this)
	);
	quitButton->setScale(1);
	quitButton->setPosition(Vec2(myXPosition + 500, myYPosition - 300));
	auto quitButtonMenu = Menu::create(quitButton, NULL);
	quitButtonMenu->setPosition(Vec2::ZERO);
	this->addChild(quitButtonMenu, 10);




	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 这些按钮只是用来测试帧动画，可以参考以下方法使用帧动画
	// 准备使用3种enemy，每种enemy只有3个动作，就是行走、攻击、死亡。

	// 添加enemy1并播放帧动画(在AppDelegate.cpp里加载好了帧动画资源）
	enemy1 = Sprite::createWithSpriteFrameName("enemy1_0.png");
	enemy1->setScale(0.8);
	Animate* enemy1Animation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy1Animation"));
	enemy1->runAction(RepeatForever::create(enemy1Animation));
	enemy1->setPosition(Vec2(myXPosition, myYPosition));
	this->addChild(enemy1, 2);

	// 添加enemy2并播放帧动画(在AppDelegate.cpp里加载好了帧动画资源）
	enemy2 = Sprite::createWithSpriteFrameName("enemy2_0.png");
	enemy2->setScale(0.8);
	Animate* enemy2Animation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy2Animation"));
	enemy2->runAction(RepeatForever::create(enemy2Animation));
	enemy2->setPosition(Vec2(myXPosition + 200, myYPosition));
	this->addChild(enemy2, 2);

	// 添加enemy3并播放帧动画(在AppDelegate.cpp里加载好了帧动画资源）
	enemy3 = Sprite::createWithSpriteFrameName("enemy3_0.png");
	enemy3->setScale(0.8);
	Animate* enemy3Animation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy3Animation"));
	enemy3->runAction(RepeatForever::create(enemy3Animation));
	enemy3->setPosition(Vec2(myXPosition - 200, myYPosition));
	this->addChild(enemy3, 2);

	// 攻击
	auto attackMenuItem = MenuItemLabel::create(Label::createWithSystemFont("attack", "fonts/Marker Felt.ttf", 50), 
		CC_CALLBACK_1(GameSence::attackMenuCallback, this));
	attackMenuItem->setPosition(Vec2(myXPosition + 500, myYPosition + 300));
	auto attackMenu = Menu::create(attackMenuItem, NULL);
	attackMenu->setPosition(Vec2::ZERO);
	this->addChild(attackMenu, 10);

	// 死亡
	auto deadMenuItem = MenuItemLabel::create(Label::createWithSystemFont("dead", "fonts/Marker Felt.ttf", 50), 
		CC_CALLBACK_1(GameSence::deadMenuCallback, this));
	deadMenuItem->setPosition(Vec2(myXPosition + 500, myYPosition + 250));
	auto deadMenu = Menu::create(deadMenuItem, NULL);
	deadMenu->setPosition(Vec2::ZERO);
	this->addChild(deadMenu, 10);

	// 行走
	auto walkMenuItem = MenuItemLabel::create(Label::createWithSystemFont("walk", "fonts/Marker Felt.ttf", 50),
		CC_CALLBACK_1(GameSence::walkMenuCallback, this));
	walkMenuItem->setPosition(Vec2(myXPosition + 500, myYPosition + 200));
	auto walkMenu = Menu::create(walkMenuItem, NULL);
	walkMenu->setPosition(Vec2::ZERO);
	this->addChild(walkMenu, 10);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return true;
}

void GameSence::attackMenuCallback(Ref* pSender) {
	enemy1->stopAllActions();
	Animate* enemy1AttackAnimation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy1AttackAnimation"));
	enemy1->runAction(enemy1AttackAnimation);

	enemy2->stopAllActions();
	Animate* enemy2AttackAnimation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy2AttackAnimation"));
	enemy2->runAction(enemy2AttackAnimation);

	enemy3->stopAllActions();
	Animate* enemy3AttackAnimation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy3AttackAnimation"));
	enemy3->runAction(enemy3AttackAnimation);
}

void GameSence::deadMenuCallback(Ref* pSender) {
	enemy1->stopAllActions();
	Animate* enemy1DeadAnimation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy1DeadAnimation"));
	enemy1->runAction(enemy1DeadAnimation);

	enemy2->stopAllActions();
	Animate* enemy2DeadAnimation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy2DeadAnimation"));
	enemy2->runAction(enemy2DeadAnimation);

	enemy3->stopAllActions();
	Animate* enemy3DeadAnimation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy3DeadAnimation"));
	enemy3->runAction(enemy3DeadAnimation);
}

void GameSence::walkMenuCallback(Ref* pSender) {
	enemy1->stopAllActions();
	Animate* enemy1Animation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy1Animation"));
	enemy1->runAction(RepeatForever::create(enemy1Animation));

	enemy2->stopAllActions();
	Animate* enemy2Animation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy2Animation"));
	enemy2->runAction(RepeatForever::create(enemy2Animation));

	enemy3->stopAllActions();
	Animate* enemy3Animation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy3Animation"));
	enemy3->runAction(RepeatForever::create(enemy3Animation));
}

void GameSence::quitCallback(Ref* pSender) {
	auto newScene = MenuScene::createScene();
	Director::getInstance()->replaceScene(CCTransitionFade::create(0.5, newScene));
}
