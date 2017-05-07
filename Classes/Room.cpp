#include"Room.h"
#include"MainScene.h"

USING_NS_CC;

Scene* Room::createScene()
{
	auto scene = Scene::create();

	auto layer = Room::create();

	scene->addChild(layer);

	return scene;
}

bool Room::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	auto bg = Sprite::create("image/bg-st.png");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(bg);
	
	_mapWindow = Sprite::create("image/map0.png");
	_mapWindow->setPosition(visibleSize.width*0.25,visibleSize.height/2);
	this->addChild(_mapWindow);

	_selectedMap = Label::createWithTTF("choose your map", "fonts/Marker Felt.ttf", 50);
	auto initMenuItem = MenuItemLabel::create(_selectedMap,CC_CALLBACK_1(Room::clickInitMenuCallBack,this));
	
	_initMenu = Menu::create(initMenuItem, nullptr);
	_initMenu->setPosition(visibleSize.width*0.75, visibleSize.height / 2);
	this->addChild(_initMenu);

	auto startLabel = Label::createWithTTF("Start", "fonts/Marker Felt.ttf", 40);
	auto startItem = MenuItemLabel::create(startLabel, CC_CALLBACK_1(Room::clickStartCallBack, this));
	auto startMenu = Menu::create(startItem, nullptr);
	startMenu->setPosition(visibleSize.width*0.8, visibleSize.height/2 - 200);
	this->addChild(startMenu);

	addMapNames();

	return true;
}

void Room::addMapNames()
{
	_mapNames.push_back("        map1");
	_mapNames.push_back("        map2");
}

void Room::clickInitMenuCallBack(Ref* obj)
{
	log("initMenu clicked");
	addSelectedMenu();
}

void Room::clickSelectedMenuCallBack(Ref* obj,int tag)
{
	log("selectedMenu clicked");
	_selectedMap->setString(_mapNames[tag]);
	changeMapWindow(tag);
	removeSelectedMenu();
}

void Room::clickStartCallBack(Ref* obj)
{
	log("hello start");
	auto scene = MainScene::createScene();
	Director::getInstance()->replaceScene(scene);
}
void Room::addSelectedMenu()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto mapLabel1 = Label::createWithTTF("map1", "fonts/Marker Felt.ttf", 50);
	auto mapItem1 = MenuItemLabel::create(mapLabel1,CC_CALLBACK_1(Room::clickSelectedMenuCallBack,this,0));
	auto mapLabel2 = Label::createWithTTF("map2", "fonts/Marker Felt.ttf", 50);
	auto mapItem2 = MenuItemLabel::create(mapLabel2, CC_CALLBACK_1(Room::clickSelectedMenuCallBack, this, 1));

	_selectedMenu = Menu::create(mapItem1, mapItem2,nullptr);
	_selectedMenu->alignItemsVertically();
	_selectedMenu->setPosition(visibleSize.width*0.75, visibleSize.height / 2 - 100);
	addChild(_selectedMenu);
}

void Room::removeSelectedMenu()
{
	removeChild(_selectedMenu);
}

void Room::changeMapWindow(int tag)
{
	removeChild(_mapWindow);
	__String* mapName = __String::createWithFormat("image/map%d.png", ++tag);
	_mapWindow = Sprite::create(mapName->getCString());
	auto visibleSize = Director::getInstance()->getVisibleSize();
	_mapWindow->setPosition(visibleSize.width*0.25, visibleSize.height / 2);
	addChild(_mapWindow);
}