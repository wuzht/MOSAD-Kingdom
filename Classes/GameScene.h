#pragma once
#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class GameSence : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void attackMenuCallback(Ref * pSender);

	void deadMenuCallback(Ref * pSender);

	void walkMenuCallback(Ref * pSender);

	void quitCallback(Ref * pSender);

	CREATE_FUNC(GameSence);

private:
	Sprite * healthBar;
	Sprite * enemy1;
	Sprite * enemy2;
	Sprite * enemy3;
	int towerLandsNum;
	std::vector<Sprite*> towerLands;
	std::vector<Vec2> towerLandsPositions;
};