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

void Bomb::boom(std::vector<std::vector<int>>& mapProp, Point coordPos)
{
	auto count = this->_bombPower;
	bool up = true, down = true, left = true, right = true;
	for (int i = 1; i <= count; ++i)
	{
		if (up)
		{
			if (mapProp[coordPos.x][coordPos.y - i] != 1)
			{
				auto sprite1 = Sprite::create("image/sample.png");
				sprite1->setAnchorPoint(Vec2(0, 0));
				sprite1->setPosition(0, 96 * i);
				this->addChild(sprite1, 100);

				if (mapProp[coordPos.x][coordPos.y - i] == 2)
					up = false;
			}
			else
				up = false;
		}
		if (down)
		{
			if (mapProp[coordPos.x][coordPos.y + i] != 1)
			{
				auto sprite2 = Sprite::create("image/sample.png");
				sprite2->setAnchorPoint(Vec2(0, 0));
				sprite2->setPosition(0, -96 * i);
				this->addChild(sprite2, 100);

				if (mapProp[coordPos.x][coordPos.y + i] == 2)
					down = false;
			}
			else
				down = false;
		}
		if (right)
		{
			if (mapProp[coordPos.x + i][coordPos.y] != 1)
			{
				auto sprite3 = Sprite::create("image/sample.png");
				sprite3->setAnchorPoint(Vec2(0, 0));
				sprite3->setPosition(96 * i, 0);
				this->addChild(sprite3, 100);

				if (mapProp[coordPos.x + i][coordPos.y] == 2)
					right = false;
			}
			else
				right = false;
		}
		if (left)
		{
			if (mapProp[coordPos.x - i][coordPos.y] != 1)
			{
				auto sprite4 = Sprite::create("image/sample.png");
				sprite4->setAnchorPoint(Vec2(0, 0));
				sprite4->setPosition(-96 * i, 0);
				this->addChild(sprite4, 100);

				if (mapProp[coordPos.x - i][coordPos.y] == 2)
					left = false;
			}
			else
				left = false;
		}
	}

	DelayTime* delayAction = DelayTime::create(0.6f);
	CallFunc* callFuncRemove = CallFunc::create(CC_CALLBACK_0(Bomb::removeFromParent, this));
	this->runAction(Sequence::createWithTwoActions(delayAction, callFuncRemove));
}