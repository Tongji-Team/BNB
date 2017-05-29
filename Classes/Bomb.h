#ifndef __BOMB_H__
#define __BOMB_H__
#include "cocos2d.h"

USING_NS_CC;

class MainScene;

class Bomb :public Sprite
{
public:
	bool init(int power);

	static Bomb* create(int power);

	void boom(MainScene* ptr, Point coordPos);

private:
	int _bombPower;
};

#endif