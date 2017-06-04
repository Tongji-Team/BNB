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

	this->initWithFile("image/player.png");

	auto animationLeft = Animation::create();
	auto animationRight = Animation::create();
	auto animationUp = Animation::create();
	auto animationDown = Animation::create();//四个方向的动作

	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
	frameCache->addSpriteFramesWithFile("_playerOne.plist", "_playerOne.png");
	for (int l = 0; l < 4; ++l)
	{
		auto frameLeft = frameCache->getSpriteFrameByName(StringUtils::format("playOne_Left%d.png", l));
		auto frameRight= frameCache->getSpriteFrameByName(StringUtils::format("playOne_Right%d.png", l));
		auto frameUp= frameCache->getSpriteFrameByName(StringUtils::format("playOne_Up%d.png", l));
		auto frameDown= frameCache->getSpriteFrameByName(StringUtils::format("playOne_Down%d.png", l));
		
		animationLeft->addSpriteFrame(frameLeft);
		animationRight->addSpriteFrame(frameRight);
		animationUp->addSpriteFrame(frameUp);
		animationDown->addSpriteFrame(frameDown);
	}
	
	animationLeft->setDelayPerUnit(3.0f / 15.0f);
	animationLeft->setRestoreOriginalFrame(true);
	_animateLeft = Animate::create(animationLeft);

	animationRight->setDelayPerUnit(3.0f / 15.0f);
	animationRight->setRestoreOriginalFrame(true);
	_animateRight = Animate::create(animationRight);

	animationUp->setDelayPerUnit(3.0f / 15.0f);
	animationUp->setRestoreOriginalFrame(true);
	_animateUp = Animate::create(animationUp);

	animationDown->setDelayPerUnit(3.0f / 15.0f);
	animationDown->setRestoreOriginalFrame(true);
	_animateDown = Animate::create(animationDown);

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

cocos2d::Animate* Player::createAnimate()//人物动画
{
	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
	frameCache->addSpriteFramesWithFile("_playerOne.plist", "_playerOne.png");

	int iFrameNum = 16;
	SpriteFrame* frame = NULL;
	Vector<SpriteFrame*>frameVec;

	while (getRight()) //人物向右运动
	{
		for (int i = 1; i <=4 ; i++)
		{
			frame = frameCache->getSpriteFrameByName(StringUtils::format("playOne_Right%d.png", i));
			frameVec.pushBack(frame);
		}
	}

	while (getDown())//人物向下运动
	{
		for (int i = 1; i <= 4; i++)
		{
			frame = frameCache->getSpriteFrameByName(StringUtils::format("playerOne_Down%d.png", i));
			frameVec.pushBack(frame);
		}
	}

	while (getLeft())//人物向左运动
	{
		for (int i = 1; i <= 4; i++)
		{
			frame = frameCache->getSpriteFrameByName(StringUtils::format("playerOne_Left%d.png", i));
			frameVec.pushBack(frame);
		}
	}

	while (getUp())//人物向上运动
	{
		for (int i = 1; i <= 4; i++)
		{
			frame = frameCache->getSpriteFrameByName(StringUtils::format("playerOne_Up%d.png", i));
			frameVec.pushBack(frame);
		}
	}
	Animation* animation = Animation::createWithSpriteFrames(frameVec);
	animation->setRestoreOriginalFrame(true);//回到第一帧开始播放
	animation->setLoops(-1);//无限循环动画
	animation->setDelayPerUnit(0.1f);//帧间隔
	
	Animation* action = Animate::create(animation);

	return action;
}