#include "Player.h"

bool Player::init()
{
	_canMove = true;
	_speed = 1;
	_bombMaxNum = 1;
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

	curPos.x -= 2;//改变精灵位置

	this->setPosition(curPos);
}

void Player::walkRight()
{
	auto curPos = this->getPosition();

	curPos.x += 2;

	this->setPosition(curPos);
}

void Player::walkDown()
{
	auto curPos = this->getPosition();

	curPos.y -= 1;

	this->setPosition(curPos);
}

void Player::walkUp()
{
	auto curPos = this->getPosition();

	curPos.y += 1;

	this->setPosition(curPos);
}