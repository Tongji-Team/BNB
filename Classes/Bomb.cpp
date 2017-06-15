#include "MainScene.h"
#include "Bomb.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

bool Bomb::init(int power)
{
	this->initWithFile("animation/_bombExplo2.png");
	this->_bombPower = power;
	bool _bombAni = false;

	auto animationBomb = Animation::create();
	SpriteFrameCache* frameCache_bomb = SpriteFrameCache::getInstance();
	frameCache_bomb->addSpriteFramesWithFile("animation/_bombAni.plist", "animation/_bombAni.png");
	for (int i = 1; i < 4; ++i)
	{
		auto frameBomb = frameCache_bomb->getSpriteFrameByName(StringUtils::format("_bombJump%d.png", i));
		animationBomb->addSpriteFrame(frameBomb);
	}
    
	animationBomb->setDelayPerUnit(1.0f / 3.0f);
	_animateBomb = RepeatForever::create(Animate::create(animationBomb));
	_animateBomb->retain();
	
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

void Bomb::boom(MainScene* ptr, Point coordPos)
{
	auto count = this->_bombPower;
	bool up = true, down = true, left = true, right = true;

	auto spriteCenter = Sprite::create("animation/_bombExplo2.png");
	spriteCenter->setAnchorPoint(Vec2(0, 0));
	spriteCenter->setPosition(0, 0);
	this->addChild(spriteCenter, 100);
	spriteCenter->setName("bomb");
	auto size = spriteCenter->getContentSize();
	auto body = PhysicsBody::createBox(Size(size.width, size.height));
	body->setCollisionBitmask(0);
	body->setContactTestBitmask(1);
	spriteCenter->setPhysicsBody(body);

    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/boom.wav", false);
	
	for (int i = 1; i <= count; ++i)
	{
		if (up)
		{
			if (coordPos.y - i > -1 && ptr->_mapProp[coordPos.x][coordPos.y - i] != 1)
			{
				auto sprite = Sprite::create("animation/_bombExplo1.png");//炸弹爆炸动画
				sprite->setRotation(90);
				sprite->setAnchorPoint(Vec2(0, 0));
				sprite->setPosition(0, 32 * (i + 1));
				this->addChild(sprite, 100);
				sprite->setName("bomb");
				auto size = sprite->getContentSize();
				auto body = PhysicsBody::createBox(Size(size.width, size.height));
				body->setCollisionBitmask(0);
				body->setContactTestBitmask(1);
				sprite->setPhysicsBody(body);

				if (ptr->_mapProp[coordPos.x][coordPos.y - i] == 4 || ptr->_mapProp[coordPos.x][coordPos.y - i] > 13)
				{
					ptr->_item->setTileGID(0, Point(coordPos.x, coordPos.y - i));
					ptr->_mapProp[coordPos.x][coordPos.y - i] -= 4;
					log("(%f,%f): %d", coordPos.x, coordPos.y - i, ptr->_mapProp[coordPos.x][coordPos.y - i]);
					up = false;
				}
			}
			else
				up = false;
		}
		if (down)
		{
			if (coordPos.y + i < 15 && ptr->_mapProp[coordPos.x][coordPos.y + i] != 1)
			{
				auto sprite = Sprite::create("animation/_bombExplo1.png");
				sprite->setRotation(90);
				sprite->setAnchorPoint(Vec2(0, 0));
				sprite->setPosition(0, -32 * (i - 1));
				this->addChild(sprite, 100);
				sprite->setName("bomb");
				auto size = sprite->getContentSize();
				auto body = PhysicsBody::createBox(Size(size.width, size.height));
				body->setCollisionBitmask(0);
				body->setContactTestBitmask(1);
				sprite->setPhysicsBody(body);

				if (ptr->_mapProp[coordPos.x][coordPos.y + i] == 4 || ptr->_mapProp[coordPos.x][coordPos.y + i] > 13)
				{
					ptr->_item->setTileGID(0, Point(coordPos.x, coordPos.y + i));
					ptr->_mapProp[coordPos.x][coordPos.y + i] -= 4;
					down = false;
				}
			}
			else
				down = false;
		}
		if (right)
		{
			if (coordPos.x + i < 15 && ptr->_mapProp[coordPos.x + i][coordPos.y] != 1)
			{
				auto sprite = Sprite::create("animation/_bombExplo1.png");
				sprite->setAnchorPoint(Vec2(0, 0));
				sprite->setPosition(32 * i, 0);
				this->addChild(sprite, 100);
				sprite->setName("bomb");
				auto size = sprite->getContentSize();
				auto body = PhysicsBody::createBox(Size(size.width, size.height));
				body->setCollisionBitmask(0);
				body->setContactTestBitmask(1);
				sprite->setPhysicsBody(body);

				if (ptr->_mapProp[coordPos.x + i][coordPos.y] == 4 || ptr->_mapProp[coordPos.x + i][coordPos.y] > 13)
				{
					ptr->_item->setTileGID(0, Point(coordPos.x + i, coordPos.y));
					ptr->_mapProp[coordPos.x + i][coordPos.y] -= 4;
					right = false;
				}
			}
			else
				right = false;
		}
		if (left)
		{
			if (coordPos.x - i > -1 && ptr->_mapProp[coordPos.x - i][coordPos.y] != 1)
			{
				auto sprite = Sprite::create("animation/_bombExplo1.png");
				sprite->setAnchorPoint(Vec2(0, 0));
				sprite->setPosition(-32 * i, 0);
				this->addChild(sprite, 100);
				sprite->setName("bomb");
				auto size = sprite->getContentSize();
				auto body = PhysicsBody::createBox(Size(size.width, size.height));
				body->setCollisionBitmask(0);
				body->setContactTestBitmask(1);
				sprite->setPhysicsBody(body);

				if (ptr->_mapProp[coordPos.x - i][coordPos.y] == 4 || ptr->_mapProp[coordPos.x - i][coordPos.y] > 13)
				{
					ptr->_item->setTileGID(0, Point(coordPos.x - i, coordPos.y));
					ptr->_mapProp[coordPos.x - i][coordPos.y] -= 4;
					left = false;
				}
			}
			else
				left = false;
		}
	}

	DelayTime* delayAction = DelayTime::create(0.6f);
	CallFunc* callFuncRemove = CallFunc::create(CC_CALLBACK_0(Bomb::removeFromParent, this));
	this->runAction(Sequence::createWithTwoActions(delayAction, callFuncRemove));
}
