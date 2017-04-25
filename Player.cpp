#include "Player.h"

bool Player::init()
{
	_canMove = true;
	_speed = 5;
	_bombMaxNum = 3;
	_bombPresentNum = 0;
	_bombPower = 1;
	_left = false;
	_right = false;
	_up = false;
	_down = false;

	this->initWithFile("image/sample.png");
	return true;
}

Player* Player::create()
{
	Player* player = new Player();
	if (player&&player->init())
	{
		player->autorelease();
		return player;
	}
	else
	{
		delete player;
		player = NULL;
		return NULL;
	}
}

void Player::walkLeft()
{
	auto curPos = this->getPosition();

	curPos.x -= _speed;//改变精灵位置

	this->setPosition(curPos);
}

void Player::walkRight()
{
	auto curPos = this->getPosition();

	curPos.x += _speed;

	this->setPosition(curPos);
}

void Player::walkDown()
{
	auto curPos = this->getPosition();

	curPos.y -= _speed;

	this->setPosition(curPos);
}

void Player::walkUp()
{
	auto curPos = this->getPosition();

	curPos.y += _speed;

	this->setPosition(curPos);
}

Bomb* Player::addBomb(int power, Vec2 pos)
{
	_bomb = Bomb::create(power);
	_bomb->setPosition(pos);
	increaseBombNum();
	DelayTime* delayAction = DelayTime::create(2.0f);
	CallFunc* callFunc = CallFunc::create(CC_CALLBACK_0(Player::decreaseBombNum, this));
	this->runAction(Sequence::createWithTwoActions(delayAction, callFunc));
	return _bomb;
}
