#include "MainScene.h"
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

void Bomb::boom(MainScene* ptr, Point coordPos)
{
	auto count = this->_bombPower;
	bool up = true, down = true, left = true, right = true;
	for (int i = 1; i <= count; ++i)
	{
		if (up)
		{
			if (ptr->_mapProp[coordPos.x][coordPos.y - i] != 1)
			{
				auto sprite = Sprite::create("image/sample.png");//Õ¨µ¯Ð§¹û
				sprite->setAnchorPoint(Vec2(0, 0));
				sprite->setPosition(0, 80 * i);
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
			if (ptr->_mapProp[coordPos.x][coordPos.y + i] != 1)
			{
				auto sprite = Sprite::create("image/sample.png");
				sprite->setAnchorPoint(Vec2(0, 0));
				sprite->setPosition(0, -80 * i);
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
			if (ptr->_mapProp[coordPos.x + i][coordPos.y] != 1)
			{
				auto sprite = Sprite::create("image/sample.png");
				sprite->setAnchorPoint(Vec2(0, 0));
				sprite->setPosition(80 * i, 0);
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
			if (ptr->_mapProp[coordPos.x - i][coordPos.y] != 1)
			{
				auto sprite = Sprite::create("image/sample.png");
				sprite->setAnchorPoint(Vec2(0, 0));
				sprite->setPosition(-80 * i, 0);
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