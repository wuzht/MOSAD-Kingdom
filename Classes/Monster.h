#ifndef MONSTER_H
#define MONSTER_H
#include "Line.h"
#include "cocos2d.h"
using std::string;
using namespace cocos2d;
struct monsterProperty {
	int health;
	float speed;
	int maxHealth;
	monsterProperty(int health, float speed)
	{
		this->health = health;
		this->speed = speed;
		this->maxHealth = health;
	}
	monsterProperty()
	{
		health = 100;
		speed = 1.0f;
		this->maxHealth = health;
	}
};

class Monster {
private:
	//怪物
	monsterProperty setting;
	Sprite* monster;
	ProgressTimer* healthBar;
	myLine line;
	int type;
public:

	bool isActive;
	Monster(Sprite* monster, ProgressTimer* healthBar, myLine line, monsterProperty setting,int type)
	{
		this->setting = setting;
		this->monster = monster;
		this->line = line;
		this->type = type;
		this->healthBar = healthBar;
		isActive = true;
	}
	//当到达终点时返回false
	bool moveForward(float delta)
	{
		Vec2 origin = monster->getPosition();
		if (line.checkEnd(origin))
		{
			return false;
		}

		line.checkNext(origin);
		Vec2 point = line.getPoint();
		Vec2 aim(point.x - origin.x, point.y - origin.y);
		aim.normalize();

		float dist = setting.speed*delta;
		monster->runAction(MoveBy::create(delta, Vec2(dist*aim.x, dist*aim.y)));
		healthBar->setPosition(origin.x+aim.x*dist, origin.y+dist*aim.y+40);
		return true;
	}
	int getType()
	{
		return type;
	}
	Sprite* getSprite()
	{
		return monster;
	}
	ProgressTimer* getHealthBar()
	{
		return healthBar;
	}
	//被攻击，如果被击杀，返回true，不然返回false
	bool beingAttacked(int attack)
	{
		setting.health -= attack;
		if (setting.health < 0)
			setting.health = 0;
		float ans = (float)setting.health / setting.maxHealth * 100;
		healthBar->setPercentage(ans);
		if (setting.health <= 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

class MonsterManager {
private:
	int deltaTime;
	float totemHealth = 100.0f;
	ProgressTimer* healthBar;
public:
	vector<Monster> storage;

	void registerHealthBar(ProgressTimer* healthBar)
	{
		this->healthBar = healthBar;
	}
	void totemAttacked()
	{
		totemHealth -= 10.0f;
		if (totemHealth < 0)
			totemHealth = 0;
		healthBar->setPercentage(totemHealth);
	}
	bool gameLose()
	{
		if (totemHealth <= 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void createMonster(string picture, Scene* scene, myLine line, monsterProperty setting)
	{
		Sprite* monster = Sprite::createWithSpriteFrameName(picture);
		monster->setScale(0.8);

		Vec2 point = line.getPoint();
		line.nextPoint();//第一个点为起始点，第二个点开始为目标点
		monster->setPosition(point.x, point.y);
		scene->addChild(monster, 5);

		int type;
		if (picture == "enemy1_0.png")
		{
			monster->stopAllActions();
			Animate* enemy1Animation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy1Animation"));
			monster->runAction(RepeatForever::create(enemy1Animation));
			type = 1;
		}
		else if (picture == "enemy2_0.png")
		{
			monster->stopAllActions();
			Animate* enemy2Animation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy2Animation"));
			monster->runAction(RepeatForever::create(enemy2Animation));
			type = 2;
		}
		else if (picture == "enemy3_0.png")
		{
			monster->stopAllActions();
			Animate* enemy3Animation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy3Animation"));
			monster->runAction(RepeatForever::create(enemy3Animation));
			type = 3;
		}

		auto sp = Sprite::create("healthBar.png");
		auto healthBar = ProgressTimer::create(sp);
		healthBar->setType(ProgressTimerType::BAR);
		healthBar->setBarChangeRate(Point(1, 0));
		healthBar->setMidpoint(Point(0, 0));
		healthBar->setPercentage(100);
		healthBar->setPosition(Vec2(point.x, point.y + 40));
		scene->addChild(healthBar, 20);

		storage.push_back(Monster(monster, healthBar,line, setting,type));
	}
	void updateAll()
	{
		for (auto iter = storage.begin();iter!=storage.end();)
		{
			bool end = false;
			if ((*iter).isActive)
			{
				bool ending = (*iter).moveForward(0.1f);
				if (!ending)
				{
					//播放攻击动画，并在攻击后消灭它
					monsterAnimate(*iter, "attack");
					totemAttacked();
					(*iter).isActive = false;
				}
			}
			else
			{
				iter = storage.erase(iter);
				end = true;
			}

			if (!end)
			{
				iter++;
			}

		}
	}
	void monsterAnimate(Monster& monster,string behaviour)
	{
		if (!monster.isActive)
			return;

		Sprite* enemy = monster.getSprite();
		auto healthBar = monster.getHealthBar();
		if (behaviour == "dead")
		{
			monster.isActive = false;
			if (monster.getType() == 1)
			{
				enemy->stopAllActions();
				Animate* enemy1DeadAnimation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy1DeadAnimation"));
				enemy->runAction(Sequence::create(enemy1DeadAnimation, CCDelayTime::create(1), CallFunc::create([enemy, healthBar]() {
					enemy->removeFromParentAndCleanup(true);
					healthBar->removeFromParentAndCleanup(true);
				}), nullptr));
			}
			else if (monster.getType() == 2)
			{
				enemy->stopAllActions();
				Animate* enemy2DeadAnimation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy2DeadAnimation"));
				enemy->runAction(Sequence::create(enemy2DeadAnimation, CCDelayTime::create(1), CallFunc::create([enemy, healthBar]() {
					enemy->removeFromParentAndCleanup(true);
					healthBar->removeFromParentAndCleanup(true);
				}), nullptr));
			}
			else if (monster.getType() == 3)
			{
				enemy->stopAllActions();
				Animate* enemy3DeadAnimation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy3DeadAnimation")); 
				enemy->runAction(Sequence::create(enemy3DeadAnimation, CCDelayTime::create(1), CallFunc::create([enemy, healthBar]() {
					enemy->removeFromParentAndCleanup(true);
					healthBar->removeFromParentAndCleanup(true);
				}), nullptr));
			}
		}
		else if (behaviour == "attack")
		{
			monster.isActive = false;
			if (monster.getType() == 1)
			{
				enemy->stopAllActions();
				Animate* enemy1AttackAnimation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy1AttackAnimation"));
				enemy->runAction(Sequence::create(enemy1AttackAnimation, CCDelayTime::create(1), CallFunc::create([enemy, healthBar]() {
					enemy->removeFromParentAndCleanup(true);
					healthBar->removeFromParentAndCleanup(true);
				}), nullptr));
			}
			else if (monster.getType() == 2)
			{
				enemy->stopAllActions();
				Animate* enemy2AttackAnimation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy2AttackAnimation"));
				enemy->runAction(Sequence::create(enemy2AttackAnimation, CCDelayTime::create(1), CallFunc::create([enemy, healthBar]() {
					enemy->removeFromParentAndCleanup(true);
					healthBar->removeFromParentAndCleanup(true);
				}), nullptr));
			}
			else if (monster.getType() == 3)
			{
				enemy->stopAllActions();
				Animate* enemy3AttackAnimation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy3AttackAnimation"));
				enemy->runAction(Sequence::create(enemy3AttackAnimation, CCDelayTime::create(1), CallFunc::create([enemy, healthBar]() {
					enemy->removeFromParentAndCleanup(true);
					healthBar->removeFromParentAndCleanup(true);
				}), nullptr));
			}
		}
	}

	bool isEmpty() {
		return storage.empty();
	}
	void stopAllActions()
	{
		for (auto iter = storage.begin(); iter != storage.end(); iter++)
		{
			auto sp = (*iter).getSprite();
			sp->stopAllActions();
		}
	}
	float getHealth() {
		return totemHealth;
	}
};
#endif
