#include "Bomb.h"

bool Bomb::init(int power)
{
	this->initWithFile("image/sample.png");
	this->_bombPower = power;
	return true;
}

Bomb* Bomb::create(int power)
{
	Bomb* bomb = new Bomb();
	if (bomb&&bomb->init(power))
	{
		bomb->autorelease();
		log("success-addBomb");
		return bomb;
	}
	else
	{
		delete bomb;
		bomb = NULL;
		return NULL;
	}
}

void Bomb::boom()
{
	this->removeFromParent();
}