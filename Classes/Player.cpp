#include "Player.h"
#include "MainScene.h"

bool Player::init()
{
	_canMove = true;
	_speed = 3;
	_bombMaxNum = 3;
	_bombPresentNum = 0;
	_bombPower = 1;
	_left = false;
	_right = false;
	_up = false;
	_down = false;
	_addBomb = false;
	_isAlive = true;

	this->initWithFile("image/player.png");

	auto size = this->getContentSize();
	auto body = PhysicsBody::createBox(Size(size.width, size.height));
	body->setCollisionBitmask(0);
	body->setContactTestBitmask(1);
	this->setPhysicsBody(body);

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

Point Player::walkLeft()
{
	auto curPos = this->getPosition();

	curPos.x -= _speed;//改变精灵位置

	return curPos;
}

Point Player::walkRight()
{
	auto curPos = this->getPosition();

	curPos.x += _speed;

	return curPos;
}

Point Player::walkDown()
{
	auto curPos = this->getPosition();

	curPos.y -= _speed;

	return curPos;
}

Point Player::walkUp()
{
	auto curPos = this->getPosition();

	curPos.y += _speed;

	return curPos;
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

bool Player::eatItem(int type)
{
	switch (type)
	{
	case 1:
		this->_bombMaxNum++;
		break;
	case 2:
		this->_speed++;
		break;
	case 3:
		this->_bombPower++;
		break;
	default:
		return false;
	}
	return true;
}