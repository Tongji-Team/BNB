#ifndef __ITEM_H__
#define __ITEM_H__
#include "cocos2d.h"

USING_NS_CC;

class Item :public Sprite
{
public:
	bool init(int type);

	static Item* create(int type);

private:
	int _type;
};

#endif