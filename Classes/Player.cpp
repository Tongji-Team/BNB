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
	_sendBomb = false;
	_isAlive = true;
	_goDown = false;
	_goLeft = false;
	_goRight = false;
	_goUp = false;
	_checkPoint = Point(0, 0);

	auto animationLeft = Animation::create();
	auto animationRight = Animation::create();
	auto animationUp = Animation::create();
	auto animationDown = Animation::create();//四个方向的动作

	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
	frameCache->addSpriteFramesWithFile("animation/_playerOne.plist", "animation/_playerOne.png");
	this->initWithSpriteFrameName("playerOne_Down2.png");
	for (int l = 1; l < 5; ++l)
	{
		auto frameLeft = frameCache->getSpriteFrameByName(StringUtils::format("playerOne_Left%d.png", l));
		auto frameRight = frameCache->getSpriteFrameByName(StringUtils::format("playerOne_Right%d.png", l));
		auto frameUp = frameCache->getSpriteFrameByName(StringUtils::format("playerOne_Up%d.png", l));
		auto frameDown = frameCache->getSpriteFrameByName(StringUtils::format("playerOne_Down%d.png", l));

		animationLeft->addSpriteFrame(frameLeft);
		animationRight->addSpriteFrame(frameRight);
		animationUp->addSpriteFrame(frameUp);
		animationDown->addSpriteFrame(frameDown);
	}

	animationLeft->setDelayPerUnit(3.0f / 15.0f);
	animationLeft->setRestoreOriginalFrame(true);
	_animateLeft = RepeatForever::create(Animate::create(animationLeft));
	_animateLeft->retain();

	animationRight->setDelayPerUnit(3.0f / 15.0f);
	animationRight->setRestoreOriginalFrame(true);
	_animateRight = RepeatForever::create(Animate::create(animationRight));
	_animateRight->retain();

	animationUp->setDelayPerUnit(3.0f / 15.0f);
	animationUp->setRestoreOriginalFrame(true);
	_animateUp = RepeatForever::create(Animate::create(animationUp));
	_animateUp->retain();

	animationDown->setDelayPerUnit(3.0f / 15.0f);
	animationDown->setRestoreOriginalFrame(true);
	_animateDown = RepeatForever::create(Animate::create(animationDown));
	_animateDown->retain();

	//以下为人物死亡动画
	auto animationDeath = Animation::create();

	SpriteFrameCache* frameDeath = SpriteFrameCache::getInstance();
	frameDeath->addSpriteFramesWithFile("animation/_playerDeath.plist", "animation/_playerDeath.png");
	for (int i = 1; i < 8; ++i)
	{
		auto frameDie = frameDeath->getSpriteFrameByName(StringUtils::format("_playerDeath%d.png", i));

		animationDeath->addSpriteFrame(frameDie);
	}
	animationDeath->setDelayPerUnit(3.0f / 15.0f);
	_animateDeath = Repeat::create(Animate::create(animationDeath), 1);
	_animateDeath->retain();

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
	if (pos == Point(-1, -1))
		return NULL;
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