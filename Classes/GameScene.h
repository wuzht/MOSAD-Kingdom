#pragma once
#include <stdio.h>
#include "cocos2d.h"
#include "Line.h"
#include "Monster.h"
#include "Tower.h"
USING_NS_CC;
using std::string;



class GameScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	CREATE_FUNC(GameScene);
	void update(float f);
	void attackMenuCallback(Ref * pSender);
	void deadMenuCallback(Ref * pSender);
	void walkMenuCallback(Ref * pSender);
	void quitCallback(Ref * pSender);
	void onMouseMove(EventMouse* event);

	void addTower1(Object* pSender);
	void addTower2(Object* pSender);
	void addTower3(Object* pSender);
	bool onTouchBegan(Touch *touch, Event* event);
	void deleteTower(Object* pSender);
	void updateTower(Object* pSender);
	void bullet(float f);
	void hitByBullet();
	void createMonster(int rate);
	void gameWin();
	void gameLose();
	void modifyMoney(int type);
	void saveScore();
private:
	MonsterManager monsterManager;
	TowerManager towerManager;
	ProgressTimer * healthBar;
	Sprite * enemy1;
	Sprite * enemy2;
	Sprite * enemy3;
	int towerLandsNum;
	std::vector<Sprite*> towerLands;
	std::vector<Vec2> towerLandsPositions;

	//�����������
	int rate;//��¼��Ϸ�Ĳ���
	myLine line1, line2, line3, line4; //������·
	int ct;//��¼ʱ��
	bool is_create;//�Ƿ񴴽�����

	//���������
	int clickItemtype;//-1Ϊ�գ����������ſ���
	Sprite* readyItem = NULL;

	//�����˵�
	Menu* towerMenu;
	Vec2 menuPos = Vec2::ZERO;
	//������
	Vec2 prevPos;
	bool isQuit = false;

	//�ӵ�
	std::vector<Sprite*> bullets;
	std::vector<int> bulletAttack;
	bool judgingBullets = false;

	//��Ǯ
	int money = 400;
	int score = 0;
	Label* rateNum;
	Label* moneyNum;
	Label* scoreNum;
	//��Ϸ�������
	bool isGameOver;
};