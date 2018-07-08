#include "GameScene.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

// ��ΪͼƬ�ز�������1920*1080�ֱ��ʴ�����ģ�������Ϸ���ڴ�СΪ1280*920����Ҫ����scale
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

	// ��������
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("sounds/Jungle_Battle.mp3");
	audio->playBackgroundMusic("sounds/Jungle_Battle.mp3", true);

	// ����ͼƬ
	auto gameBg = Sprite::create("GameBg.png");
	gameBg->setScale(MY_SCALE_SIZE);
	gameBg->setPosition(Vec2(myXPosition, myYPosition));
	this->addChild(gameBg, 0);

	// Ѫ�������Ǹ����
	auto healthBarContaier = Sprite::create("HealthBar1.png");
	healthBarContaier->setScale(1, 0.5);
	healthBarContaier->setPosition(Vec2(myXPosition, myYPosition + 320));
	this->addChild(healthBarContaier, 10);

	// Ѫ����ͼ�ڵ�Ѫ����ͼ��Ѫ��Ϊ0����Ϸ������
	healthBar = Sprite::create("HealthBar2.png");
	healthBar->setScale(1, 0.5);
	healthBar->setPosition(Vec2(myXPosition, myYPosition + 313));
	this->addChild(healthBar, 9);

	// ͼ��
	auto totem = Sprite::create("Totem.png");
	totem->setScale(MY_SCALE_SIZE);
	totem->setPosition(Vec2(myXPosition, myYPosition - 300));
	this->addChild(totem, 2);

	// ͼ���ӵ�
	auto totemBullet = Sprite::create("TotemBullet.png");
	totemBullet->setScale(MY_SCALE_SIZE);
	// ����ͼ���ӵ��������ʼλ��
	totemBullet->setPosition(Vec2(myXPosition + 16, myYPosition - 258));
	this->addChild(totemBullet, 3);

	// ��λ����
	towerLandsNum = 12;

	// ������λ��λ��
	// ÿ����ɫ��Ϊһ�飬�������λλ�õ�˳���Ǵ�����
	// ���Ͽ�
	towerLandsPositions.push_back(Vec2(myXPosition - 530, myYPosition + 165));	// 0
	towerLandsPositions.push_back(Vec2(myXPosition - 210, myYPosition + 150));	// 1
	towerLandsPositions.push_back(Vec2(myXPosition- 10, myYPosition + 230));	// 2

	towerLandsPositions.push_back(Vec2(myXPosition + 55, myYPosition + 20));	// 3
	towerLandsPositions.push_back(Vec2(myXPosition + 180, myYPosition - 100));	// 4

	// ���
	towerLandsPositions.push_back(Vec2(myXPosition - 260, myYPosition + 10));	// 5

	// ���¿�
	towerLandsPositions.push_back(Vec2(myXPosition - 320, myYPosition - 120));	// 6
	towerLandsPositions.push_back(Vec2(myXPosition - 100, myYPosition - 100));	// 7
	towerLandsPositions.push_back(Vec2(myXPosition - 30, myYPosition - 150));	// 8

	// ���Ͽ�
	towerLandsPositions.push_back(Vec2(myXPosition + 180, myYPosition + 180));	// 9
	towerLandsPositions.push_back(Vec2(myXPosition + 310, myYPosition + 60));	// 10

	// ���¿�
	towerLandsPositions.push_back(Vec2(myXPosition + 390, myYPosition - 95));	// 11

	// ������λ
	for (int i = 0; i < towerLandsNum; i++)
	{
		auto towerLand = Sprite::create("TowerLand.png");
		towerLand->setScale(MY_SCALE_SIZE);
		towerLand->setPosition(towerLandsPositions[i]);
		// �ŵ�towerLands��Ա���洦���ɸ�����Ҫ�޸�
		towerLands.push_back(towerLand);
		this->addChild(towerLand, 1);
	}

	// ���Ͷ�Ӧ���ӵ���ע���������scaleֵ��λ��ƫ�����������ã���Ҫ��������ֵ��ʾ����
	auto tower1 = Sprite::create("tower1.png");
	tower1->setScale(0.7);
	// ������λ��λ�õ������������ע�͡�����λ��Ҫ����һ��ƫ������Vec2(-2, 20)��
	tower1->setPosition(towerLandsPositions[4] + Vec2(-2, 16));
	this->addChild(tower1, 2);
	// �ӵ�
	auto tower1Bullet = Sprite::create("tower1Bullet.png");
	tower1Bullet->setScale(0.8);
	tower1Bullet->setPosition(towerLandsPositions[4] + Vec2(-2, 50));
	this->addChild(tower1Bullet, 3);

	auto tower2 = Sprite::create("tower2.png");
	tower2->setScale(0.8);
	tower2->setPosition(towerLandsPositions[8] + Vec2(-4, 20));
	this->addChild(tower2, 2);
	// �ӵ�
	auto tower2Bullet = Sprite::create("tower2Bullet.png");
	tower2Bullet->setScale(0.5);
	tower2Bullet->setPosition(towerLandsPositions[8] + Vec2(-4, 60));
	this->addChild(tower2Bullet, 3);

	auto tower3 = Sprite::create("tower3.png");
	tower3->setScale(0.8);
	tower3->setPosition(towerLandsPositions[3] + Vec2(0, 4));
	this->addChild(tower3, 2);
	// �ӵ�
	auto tower3Bullet = Sprite::create("tower3Bullet.png");
	tower3Bullet->setScale(0.8);
	tower3Bullet->setPosition(towerLandsPositions[3] + Vec2(-4, 20));
	this->addChild(tower3Bullet, 3);

	// �������˵���ť
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
	// ��Щ��ťֻ����������֡���������Բο����·���ʹ��֡����
	// ׼��ʹ��3��enemy��ÿ��enemyֻ��3���������������ߡ�������������

	// ���enemy1������֡����(��AppDelegate.cpp����غ���֡������Դ��
	enemy1 = Sprite::createWithSpriteFrameName("enemy1_0.png");
	enemy1->setScale(0.8);
	Animate* enemy1Animation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy1Animation"));
	enemy1->runAction(RepeatForever::create(enemy1Animation));
	enemy1->setPosition(Vec2(myXPosition, myYPosition));
	this->addChild(enemy1, 2);

	// ���enemy2������֡����(��AppDelegate.cpp����غ���֡������Դ��
	enemy2 = Sprite::createWithSpriteFrameName("enemy2_0.png");
	enemy2->setScale(0.8);
	Animate* enemy2Animation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy2Animation"));
	enemy2->runAction(RepeatForever::create(enemy2Animation));
	enemy2->setPosition(Vec2(myXPosition + 200, myYPosition));
	this->addChild(enemy2, 2);

	// ���enemy3������֡����(��AppDelegate.cpp����غ���֡������Դ��
	enemy3 = Sprite::createWithSpriteFrameName("enemy3_0.png");
	enemy3->setScale(0.8);
	Animate* enemy3Animation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy3Animation"));
	enemy3->runAction(RepeatForever::create(enemy3Animation));
	enemy3->setPosition(Vec2(myXPosition - 200, myYPosition));
	this->addChild(enemy3, 2);

	// ����
	auto attackMenuItem = MenuItemLabel::create(Label::createWithSystemFont("attack", "fonts/Marker Felt.ttf", 50), 
		CC_CALLBACK_1(GameSence::attackMenuCallback, this));
	attackMenuItem->setPosition(Vec2(myXPosition + 500, myYPosition + 300));
	auto attackMenu = Menu::create(attackMenuItem, NULL);
	attackMenu->setPosition(Vec2::ZERO);
	this->addChild(attackMenu, 10);

	// ����
	auto deadMenuItem = MenuItemLabel::create(Label::createWithSystemFont("dead", "fonts/Marker Felt.ttf", 50), 
		CC_CALLBACK_1(GameSence::deadMenuCallback, this));
	deadMenuItem->setPosition(Vec2(myXPosition + 500, myYPosition + 250));
	auto deadMenu = Menu::create(deadMenuItem, NULL);
	deadMenu->setPosition(Vec2::ZERO);
	this->addChild(deadMenu, 10);

	// ����
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
