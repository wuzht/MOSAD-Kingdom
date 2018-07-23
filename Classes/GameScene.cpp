#include "GameScene.h"
#include "MenuScene.h"
#include "Line.h"
#include "SimpleAudioEngine.h"
#include "chineseDisplay.h"
#define database UserDefault::getInstance()
USING_NS_CC;
// ��ΪͼƬ�ز�������1920*1080�ֱ��ʴ�����ģ�������Ϸ���ڴ�СΪ1280*920����Ҫ����scale
#define MY_SCALE_SIZE 0.54
Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	isQuit = false;
	isGameOver = false;
	float myXPosition = visibleSize.width / 2 + origin.x;
	float myYPosition = visibleSize.height / 2 + origin.y;

	//�����ã���ʾ�������
	auto listener = EventListenerMouse::create();
	listener->onMouseMove = CC_CALLBACK_1(GameScene::onMouseMove, this);
	_eventDispatcher->addEventListenerWithFixedPriority(listener, 1);
	//�����¼�
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
		int code = (int)keyCode;
		if (code == 6 && this->readyItem!=NULL)
		{
			this->readyItem->removeFromParentAndCleanup(true);
			this->readyItem = NULL;
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	// �������� ��������Ч
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("sounds/Jungle_Battle.mp3");
	audio->playBackgroundMusic("sounds/Jungle_Battle.mp3", true);
	audio->preloadEffect("sounds/Sound_Sorcerer.wav");
	audio->preloadEffect("sounds/savant_attack.wav");
	audio->preloadEffect("sounds/dwaarp_attack.wav");
	audio->preloadEffect("sounds/archmage_attack.wav");

	// ����ͼƬ
	auto gameBg = Sprite::create("GameBg.png");
	gameBg->setScale(MY_SCALE_SIZE);
	gameBg->setPosition(Vec2(myXPosition, myYPosition));
	this->addChild(gameBg, 0);

	// Ѫ�������Ǹ����
	auto healthBarContaier = Sprite::create("HealthBar1.png");
	healthBarContaier->setScale(1, 0.7);
	healthBarContaier->setPosition(Vec2(myXPosition, myYPosition + 320));
	this->addChild(healthBarContaier, 10);

	// Ѫ����ͼ�ڵ�Ѫ����ͼ��Ѫ��Ϊ0����Ϸ������
	auto sp = Sprite::create("HealthBar2.png");
	healthBar = ProgressTimer::create(sp);
	healthBar->setType(ProgressTimerType::BAR);
	healthBar->setBarChangeRate(Point(1, 0));
	healthBar->setMidpoint(Point(0, 0));
	healthBar->setPercentage(100);
	healthBar->setPosition(Vec2(myXPosition, myYPosition + 313));
	this->addChild(healthBar, 9);
	monsterManager.registerHealthBar(healthBar);

	//����ͼ��
	towerManager.createTower("Totem.png", TowerProperty(50, 500, 300,30), Vec2(myXPosition, myYPosition - 300),this);
	
	//��ѡ��˵�
	readyItem = NULL;
	towerMenu = NULL;
	clickItemtype = -1;
	prevPos = Vec2(0, 0);

	auto tower1 = MenuItemImage::create(
		"tower1.png", 
		"tower1.png", 
		CC_CALLBACK_1(GameScene::addTower1, this));
	tower1->setScale(0.7);
	tower1->setPosition(Vec2(visibleSize.width - 150, 150));
	auto moneyLabel1 = Sprite::create("moneyLabel.png");
	moneyLabel1->setScale(0.4);
	moneyLabel1->setPosition(Vec2(visibleSize.width - 80, 150));
	this->addChild(moneyLabel1, 6);
	auto towerMoney1 = Label::createWithTTF("200", "fonts/Comic_Book.TTF", 24);
	towerMoney1->setColor(Color3B(255, 255, 255));
	towerMoney1->setPosition(Vec2(visibleSize.width - 35, 150));
	this->addChild(towerMoney1, 6);

	auto tower2 = MenuItemImage::create(
		"tower2.png",
		"tower2.png",
		CC_CALLBACK_1(GameScene::addTower2, this));
	tower2->setScale(0.8);
	tower2->setPosition(Vec2(visibleSize.width - 150, 250));
	auto moneyLabel2 = Sprite::create("moneyLabel.png");
	moneyLabel2->setScale(0.4);
	moneyLabel2->setPosition(Vec2(visibleSize.width - 80, 250));
	this->addChild(moneyLabel2, 6);
	auto towerMoney2 = Label::createWithTTF("200", "fonts/Comic_Book.TTF", 24);
	towerMoney2->setColor(Color3B(255, 255, 255));
	towerMoney2->setPosition(Vec2(visibleSize.width - 35, 250));
	this->addChild(towerMoney2, 6);

	auto tower3 = MenuItemImage::create(
		"tower3.png",
		"tower3.png",
		CC_CALLBACK_1(GameScene::addTower3, this));
	tower3->setScale(0.8);
	tower3->setPosition(Vec2(visibleSize.width - 150, 330));
	auto moneyLabel3 = Sprite::create("moneyLabel.png");
	moneyLabel3->setScale(0.4);
	moneyLabel3->setPosition(Vec2(visibleSize.width - 80, 330));
	this->addChild(moneyLabel3, 6);
	auto towerMoney3 = Label::createWithTTF("200", "fonts/Comic_Book.TTF", 24);
	towerMoney3->setColor(Color3B(255, 255, 255));
	towerMoney3->setPosition(Vec2(visibleSize.width - 35, 330));
	this->addChild(towerMoney3, 6);

	auto menu = Menu::create(tower1, tower2, tower3, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 5);

	//���յ���Ϊ�����������������Ϊ�ڿյ��ڲ���ת��
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition - 530, myYPosition + 165), this);	// 0
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition - 210, myYPosition + 150), this);	// 1
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition - 10, myYPosition + 230), this);	// 2

	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition + 55, myYPosition + 20), this);	// 3
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition + 180, myYPosition - 100), this);	// 4

																				// ���
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition - 260, myYPosition + 10), this);	// 5

																				// ���¿�
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition - 320, myYPosition - 120), this);	// 6
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition - 100, myYPosition - 100), this);	// 7
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition - 30, myYPosition - 150), this);	// 8

																				// ���Ͽ�
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition + 180, myYPosition + 180), this);	// 9
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition + 310, myYPosition + 60), this);	// 10

																				// ���¿�
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition + 390, myYPosition - 95), this);	// 11
	// �������˵���ť
	auto quitButton = MenuItemImage::create(
		"QuitNormal.png",
		"QuitSelected.png",
		CC_CALLBACK_1(GameScene::quitCallback, this)
	);
	quitButton->setScale(1);
	quitButton->setPosition(Vec2(myXPosition + 500, myYPosition - 300));
	auto quitButtonMenu = Menu::create(quitButton, NULL);
	quitButtonMenu->setPosition(Vec2::ZERO);
	this->addChild(quitButtonMenu, 10);
	//line1
	line1.addPoint(0, 189);
	line1.addPoint(100, 212);
	line1.addPoint(200, 259);
	line1.addPoint(243, 277);
	line1.addPoint(300, 300);
	line1.addPoint(374, 308);
	line1.addPoint(400, 300);
	line1.addPoint(500, 339);
	line1.addPoint(600, 318);
	line1.addPoint(700, 245);
	line1.addPoint(735, 183);
	line1.addPoint(myXPosition, myYPosition - 300);
	//line2;
	line2.addPoint(0, 493);
	line2.addPoint(100, 441);
	line2.addPoint(200, 450);
	line2.addPoint(300, 460);
	line2.addPoint(400, 432);
	line2.addPoint(500, 339);
	line2.addPoint(600, 318);
	line2.addPoint(700, 245);
	line2.addPoint(735, 183);
	line2.addPoint(myXPosition, myYPosition - 300);
	//line3;
	line3.addPoint(757, 698);
	line3.addPoint(732, 619);
	line3.addPoint(737, 495);
	line3.addPoint(801, 409);
	line3.addPoint(916, 303);
	line3.addPoint(916, 199);
	line3.addPoint(735, 183);
	line3.addPoint(myXPosition, myYPosition - 300);
	//line4;
	line4.addPoint(1244, 443);
	line4.addPoint(1077, 436);
	line4.addPoint(916, 303);
	line4.addPoint(916, 199);
	line4.addPoint(735, 183);
	line4.addPoint(myXPosition, myYPosition - 300);

	//monsterManager.createMonster("enemy3_0.png",this, line3, monsterProperty(100,50));
	//monsterManager.createMonster("enemy3_0.png", this, line3, monsterProperty(100, 50));
	//monsterManager.createMonster("enemy3_0.png", this, line3, monsterProperty(100, 50));
	//monsterManager.createMonster("enemy1_0.png", this, line4, monsterProperty(100, 50));

	//������һ������
	rate = 0;
	createMonster(rate);
	is_create = true;
	ct = 0;

	//��ʾ��Ҹ�����
	rateNum = Label::createWithTTF("1/10", "fonts/Comic_Book.TTF", 24);
	rateNum->setColor(Color3B(255, 255, 255));
	rateNum->setPosition(visibleSize.width - 195, visibleSize.height - 100);
	this->addChild(rateNum, 11);
	moneyNum = Label::createWithTTF("400", "fonts/Comic_Book.TTF", 24);
	moneyNum->setColor(Color3B(255, 255, 255));
	moneyNum->setPosition(visibleSize.width - 195, visibleSize.height - 50);
	this->addChild(moneyNum, 11);
	scoreNum = Label::createWithTTF("0", "fonts/Comic_Book.TTF", 24);
	scoreNum->setColor(Color3B(255, 255, 255));
	scoreNum->setPosition(visibleSize.width - 75, visibleSize.height - 50);
	this->addChild(scoreNum, 11);

	auto moneyBar = Sprite::create("moneyBar.png");
	moneyBar->setScale(0.8);
	moneyBar->setPosition(Vec2(myXPosition + 430, myYPosition + 310));
	this->addChild(moneyBar, 10);

	auto rateBar = Sprite::create("rateBar.png");
	rateBar->setScale(0.8);
	rateBar->setPosition(Vec2(myXPosition + 480, myYPosition + 260));
	this->addChild(rateBar, 10);

	auto blackBar = Sprite::create("blackBar.png");
	blackBar->setScale(0.45, 0.55);
	blackBar->setPosition(Vec2(myXPosition + 575, myYPosition + 310));
	this->addChild(blackBar, 10);

	auto scoreStar = Sprite::create("scoreStar.png");
	scoreStar->setScale(0.7);
	scoreStar->setPosition(Vec2(myXPosition + 515, myYPosition + 310));
	this->addChild(scoreStar, 10);


	//������
	schedule(schedule_selector(GameScene::update), 0.1f, kRepeatForever, 0);
	schedule(schedule_selector(GameScene::bullet), 0.1f, kRepeatForever, 0);
	//�¼�������
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, gameBg);
	return true;
}

void GameScene::update(float f)
{
	if (isGameOver)
		return;

	if (monsterManager.gameLose())
	{
		gameLose();
	}



	monsterManager.updateAll();
	hitByBullet();

	if (is_create) {
		ct++;
		int speed = 10 + 10 * ((rate + 1) / 2);
		int dalaytime = 800 / speed;
		int temp = ct / dalaytime;
		if (temp >= (3 + rate / 2)) {
			is_create = false;
			ct = 0;
		}
		else if (ct % dalaytime == 0) createMonster(rate);
	}
	else {	//�ж�������һ���¹��޻�����Ϸ����
		ct++;
		if (monsterManager.isEmpty() && rate > 9) {
			gameWin();
		}
		if (ct == 100) {
			ct = 0;
			is_create = true;
			rate++;
			char str[15];
			if (rate > 10) sprintf(str, "%d/10", rate);
			else sprintf(str, "%d/10", rate + 1);
			rateNum->setString(str);
		}
	}
}

void GameScene::onMouseMove(EventMouse* event)
{
	Vec2 x = event->getLocationInView();
	//CCLOG("%f,%f", x.x,x.y);
	if (readyItem != NULL && !isQuit)
	{
		readyItem->setPosition(x.x, x.y);
	}
	x = event->getLocation();
	x = event->getLocationInView();
	x = event->getPreviousLocation();
	x = event->getPreviousLocationInView();
	x = event->getStartLocation();
	x = event->getStartLocationInView();
	x = event->getDelta();
}

void GameScene::attackMenuCallback(Ref* pSender) {
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

void GameScene::deadMenuCallback(Ref* pSender) {
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

void GameScene::walkMenuCallback(Ref* pSender) {
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

void GameScene::quitCallback(Ref* pSender) {
	isQuit = true;
	if(!isGameOver)
		saveScore();
	auto newScene = MenuScene::createScene();
	Director::getInstance()->replaceScene(CCTransitionFade::create(0.5, newScene));
}


void GameScene::addTower1(Object* pSender)
{
	if (isGameOver)
		return;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	if (readyItem != NULL)
	{
		readyItem->removeFromParentAndCleanup(true);
		readyItem = NULL;
	}
	clickItemtype = 1;
	readyItem = Sprite::create("tower1.png");
	readyItem->setScale(0.7);
	readyItem->setPosition(visibleSize.width - 100, 150);
	this->addChild(readyItem,15);
}
void GameScene::addTower2(Object* pSender)
{
	if (isGameOver)
		return;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	if (readyItem != NULL)
	{
		readyItem->removeFromParentAndCleanup(true);
		readyItem = NULL;
	}
	clickItemtype = 2;
	readyItem = Sprite::create("tower2.png");
	readyItem->setScale(0.8);
	readyItem->setPosition(visibleSize.width - 100, 250);
	this->addChild(readyItem, 15);
}
void GameScene::addTower3(Object* pSender)
{
	if (isGameOver)
		return;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	if (readyItem != NULL)
	{
		readyItem->removeFromParentAndCleanup(true);
		readyItem = NULL;
	}
	clickItemtype = 3;
	readyItem = Sprite::create("tower3.png");
	readyItem->setScale(0.8);
	readyItem->setPosition(visibleSize.width - 100, 330);
	this->addChild(readyItem, 15);
}

bool GameScene::onTouchBegan(Touch *touch, Event* event)
{
	if (isGameOver)
		return false;

	CCLOG("%f,%f", touch->getLocation().x, touch->getLocation().y);
	if (prevPos.getDistance(touch->getLocation()) < 1)
	{
		return false;
	}
	bool isClick = false;
	bool clickingMenu = false;
	if (towerManager.clickTower(touch->getLocation()))
	{
		isClick = true;
		Tower clickingTower = towerManager.getTowerThroughPos(touch->getLocation());
		if (clickingTower.getType() == 4)//���ͼ��
		{
			if (towerMenu != NULL)
			{
				towerMenu->removeFromParentAndCleanup(true);
				towerMenu = NULL;
			}
			return false;

		}
		else if (clickingTower.isGround() && clickItemtype != -1)//������ǿյ��Ҵ�ѡ��ǿ�
		{
			if (money < 200)
				return false;
			else
				modifyMoney(1);
			towerManager.changeTower(touch->getLocation(), clickItemtype);

			clickItemtype = -1;
			readyItem->removeFromParentAndCleanup(true);
			readyItem = NULL;
		}
		else if (!clickingTower.isGround() && towerMenu == NULL) //չ���˵�
		{
			auto update = MenuItemImage::create("update.png", "update.png", CC_CALLBACK_1(GameScene::updateTower, this));
			update->setScale(0.4);
			update->setPosition(Vec2(-30, 50));
			auto deleting = MenuItemImage::create("delete.png", "delete.png", CC_CALLBACK_1(GameScene::deleteTower, this));
			deleting->setScale(0.4);
			deleting->setPosition(Vec2(30, 50));
			auto money = MenuItemLabel::create(Label::createWithSystemFont("200", "����", 18));
			money->setColor(Color3B(255, 255, 0));
			money->setPosition(-30, 80);

			towerMenu = Menu::create(update, deleting,money, NULL);
			towerMenu->setPosition(clickingTower.getPosition());
			this->addChild(towerMenu, 20);
			clickingMenu = true;
			menuPos = touch->getLocation();
		}
		else if (!clickingTower.isGround() && towerMenu != NULL)
		{
			towerMenu->removeFromParentAndCleanup(true);
			auto update = MenuItemImage::create("update.png", "update.png", CC_CALLBACK_1(GameScene::updateTower, this));
			update->setScale(0.4);
			update->setPosition(Vec2(-30, 50));
			auto deleting = MenuItemImage::create("delete.png", "delete.png", CC_CALLBACK_1(GameScene::deleteTower, this));
			deleting->setScale(0.4);
			deleting->setPosition(Vec2(30, 50));
			auto money = MenuItemLabel::create(Label::createWithSystemFont("200", "����", 18));
			money->setColor(Color3B(255, 255, 0));
			money->setPosition(-30, 80);

			towerMenu = Menu::create(update, deleting, money, NULL);
			towerMenu->setPosition(clickingTower.getPosition());
			this->addChild(towerMenu, 20);
			clickingMenu = true;
			menuPos = touch->getLocation();
		}
	}

	if (!clickingMenu && towerMenu != NULL)//������˲˵�����κεط����ᵼ�²˵�����
	{
		towerMenu->removeFromParentAndCleanup(true);
		towerMenu = NULL;
	}
	prevPos = touch->getLocation();
	return isClick;
}

void GameScene::deleteTower(Object* pSender)
{
	towerManager.deleteTower(menuPos);
	modifyMoney(4);
	towerMenu->removeFromParentAndCleanup(true);
	towerMenu = NULL;
}
void GameScene::updateTower(Object* pSender)
{
	if (money >= 200) {
		modifyMoney(1);
		towerManager.updateTower(menuPos);
	}
	towerMenu->removeFromParentAndCleanup(true);
	towerMenu = NULL;
}

//�¼�
//�����ӵ�
void GameScene::bullet(float f)
{
	if (isGameOver)
		return;
	Sprite* Smonster;
	Sprite* Stower;
	for (auto iter = towerManager.tower.begin(); iter != towerManager.tower.end(); iter++)
	{
		Stower = (Sprite*)(*iter).getTower();
		if ((*iter).getType() == 0)
			continue;

		(*iter).counting();
		if (!(*iter).isFire())
		{
			continue;
		}

		for (auto iter1 = monsterManager.storage.begin(); iter1 != monsterManager.storage.end(); iter1++)
		{
			Smonster = (Sprite*)(*iter1).getSprite();
			//�жϹ��������ľ��룬���С��������̣���ô�����ӵ�
			//if (sqrt(pow(Smonster->getPosition().x - Stower->getPosition().x, 2) + pow(Smonster->getPosition().y - Stower->getPosition().y, 2))<(*iter).getTowerProperty().range)
			if(Smonster->getPosition().getDistance(Stower->getPosition())<(*iter).getTowerProperty().range)
			{
				string picture = "";
				float offsetx, offsety, scale;
				switch ((*iter).getType())
				{
				case 1:
					picture = "tower1Bullet.png";
					offsetx = -2;
					offsety = 40;
					scale = 0.8;
					// ������Ч
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/dwaarp_attack.wav", false);			
					break;
				case 2:
					picture = "tower2Bullet.png";
					offsetx = -4;
					offsety = 40;
					scale = 0.5;
					// ������Ч
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/Sound_Sorcerer.wav", false);
					break;
				case 3:
					picture = "tower3Bullet.png";
					offsetx = -4;
					offsety = 13;
					scale = 0.8;
					// ������Ч
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/archmage_attack.wav", false);
					break;
				case 4:
					picture = "TotemBullet.png";
					offsetx = 16;
					offsety = 50;
					scale = 0.8;
					// ������Ч
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/savant_attack.wav", false);
					break;
				}
				auto towerBullet = Sprite::create(picture);
				towerBullet->setScale(scale);
				towerBullet->setPosition(Stower->getPosition() + Vec2(offsetx, offsety));
				auto time = Smonster->getPosition().getDistance(Stower->getPosition()) / (*iter).getTowerProperty().speed;

				towerBullet->runAction(Sequence::create(MoveTo::create(time, Vec2(Smonster->getPosition())), FadeOut::create(time), nullptr));
				
				while (judgingBullets)//�����ж���ײ��ʱ���ӳٷ����ӵ�����������
				{
				}
				bullets.push_back(towerBullet);
				bulletAttack.push_back((*iter).getAttack());
				this->addChild(towerBullet, 11);
				break;
			}
		}

	}

}
//�ӵ����й���
void GameScene::hitByBullet()
{
	Sprite* Smonster;
	judgingBullets = true;
	for (auto iter1 = monsterManager.storage.begin(); iter1 != monsterManager.storage.end(); iter1++)
	{
		Smonster = (Sprite*)(*iter1).getSprite();

		auto iter3 = bulletAttack.begin();
		for (auto iter2 = bullets.begin(); iter2 != bullets.end(); )
		{
			if (Smonster->getBoundingBox().containsPoint((*iter2)->getPosition()))//��ը��ɾ���ӵ�
			{
				//��ʱ���������Ѫ�����ٵĲ���
				Sprite* boomBullet = (*iter2);
				boomBullet->stopAllActions();
				boomBullet->runAction(
					Sequence::create(
						Animate::create(AnimationCache::getInstance()->getAnimation("explodeAnimation")),
						CallFunc::create([boomBullet] {
							boomBullet->removeFromParentAndCleanup(true);
						}),
						nullptr));

				//����ɱ
				if ((*iter1).beingAttacked(*iter3))
				{
					monsterManager.monsterAnimate((*iter1), "dead");
					modifyMoney(0);
				}
				;

				iter2 = bullets.erase(iter2);
				iter3 = bulletAttack.erase(iter3);
			}
			else
			{
				iter2++;
				iter3++;
			}
		}
	}
	judgingBullets = false;

}

void GameScene::createMonster(int rate)
{
	//int num = 3 + rate/2;
	int speed = 30 + 10 * ((rate + 1) / 2);
	monsterManager.createMonster("enemy1_0.png", this, line1, monsterProperty(100, speed));
	monsterManager.createMonster("enemy2_0.png", this, line2, monsterProperty(100, speed));
	monsterManager.createMonster("enemy3_0.png", this, line3, monsterProperty(100, speed));
}

void GameScene::gameWin()
{
	if (!isGameOver)
	{
		isGameOver = true;
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		auto win = Sprite::create("win.png");
		win->setPosition(visibleSize.width / 2, visibleSize.height / 2);
		win->setScale(0.6);
		this->addChild(win, 100);
		char font[10] = { "ʤ��" };
		String str = chineseDisplay::a(font);
		Label* label = Label::createWithSystemFont(str.getCString(), "����", 60);
		label->setPosition(visibleSize.width / 2, visibleSize.height / 2);
		this->addChild(label, 101);

		//����ϵͳ
		auto ps = ParticleFireworks::create();
		ps->setPosition(300, 100);
		ps->setLife(1.5f);
		ps->setSpeed(300);
		ps->setStartColor(ccc4f(1, (float)251 / 255, (float)21 / 255, 1));
		ps->setEndColor(ccc4f(1, (float)251 / 255, (float)21 / 255, 1));
		this->addChild(ps, 50);
		auto ps2 = ParticleFireworks::create();
		ps2->setPosition(visibleSize.width - 300, 100);
		ps2->setLife(1.5f);
		ps2->setSpeed(300);
		ps2->setStartColor(ccc4f(1, (float)251 / 255, (float)21 / 255, 1));
		ps2->setEndColor(ccc4f(1, (float)251 / 255, (float)21 / 255, 1));
		this->addChild(ps2, 50);
		saveScore();
	}
}

void GameScene::gameLose()
{
	if (!isGameOver)
	{
		CCLOG("game lose");
		isGameOver = true;
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		for (auto iter = bullets.begin(); iter != bullets.end(); iter++)
		{
			(*iter)->stopAllActions();
		}
		monsterManager.stopAllActions();

		auto lose = Sprite::create("lose.png");
		lose->setPosition(visibleSize.width / 2, visibleSize.height / 2);
		lose->setScale(0.6);
		this->addChild(lose, 100);

		char font[10] = { "ʧ��" };
		String str = chineseDisplay::a(font);
		Label* label = Label::createWithSystemFont(str.getCString(), "����", 60);
		label->setPosition(visibleSize.width / 2, visibleSize.height / 2);
		this->addChild(label, 101);
		saveScore();
		//��Ϸ����
	}
}

//�޸Ľ��
void GameScene::modifyMoney(int type)
{
	switch (type)
	{
		//��ɱ����
	case 0:
		money += 20;
		score += 20;
		break;

		//����1������
	case 1:
		money -= 200;
		break;

		//����2������
	case 2:
		money -= 200;
		break;

		//����3������
	case 3:
		money -= 200;
		break;
		//������Ǯ
	case 4:
		money += 100;
		break;
	default:
		break;
	}

	char str[15];
	sprintf(str, "%d", money);
	moneyNum->setString(str);

	sprintf(str, "%d", score);
	scoreNum->setString(str);
}

void GameScene::saveScore()
{
	score = score + money * 5 + monsterManager.getHealth() * 20;
	int scoreNum[10];
	scoreNum[0] = database->getIntegerForKey("one");
	scoreNum[1] = database->getIntegerForKey("two");
	scoreNum[2] = database->getIntegerForKey("three");
	scoreNum[3] = database->getIntegerForKey("four");
	scoreNum[4] = database->getIntegerForKey("five");
	scoreNum[5] = database->getIntegerForKey("six");
	scoreNum[6] = database->getIntegerForKey("seven");
	scoreNum[7] = database->getIntegerForKey("eight");
	scoreNum[8] = database->getIntegerForKey("nine");
	scoreNum[9] = database->getIntegerForKey("ten");

	int changer = score;
	int temp = 0;
	for (int i = 0; i < 10; i++) {
		if (score > scoreNum[i]) {
			temp = scoreNum[i];
			scoreNum[i] = changer;
			changer = temp;
		}
	}

	database->setBoolForKey("isExist", true);
	database->setIntegerForKey("one", scoreNum[0]);
	database->setIntegerForKey("two", scoreNum[1]);
	database->setIntegerForKey("three", scoreNum[2]);
	database->setIntegerForKey("four", scoreNum[3]);
	database->setIntegerForKey("five", scoreNum[4]);
	database->setIntegerForKey("six", scoreNum[5]);
	database->setIntegerForKey("seven", scoreNum[6]);
	database->setIntegerForKey("eight", scoreNum[7]);
	database->setIntegerForKey("nine", scoreNum[8]);
	database->setIntegerForKey("ten", scoreNum[9]);
}
