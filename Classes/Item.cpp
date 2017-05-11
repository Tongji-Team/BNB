#include "Item.h"

bool Item::init(int type)
{
	switch (type)
	{
	case 1:
		this->initWithFile("image/bomb.png");
		break;
	case 2:
		this->initWithFile("image/shoe.png");
		break;
	case 3:
		this->initWithFile("image/speed.png");
		break;
	default:
		return false;
	}

	this->_type = type;
	return true;
}

Item* Item::create(int type)
{
	Item* item = new Item();
	if (item&&item->init(type))
	{
		item->autorelease();
		log("success-addItem");
		return item;
	}
	else
	{
		delete item;
		item = NULL;
		return NULL;
	}
}