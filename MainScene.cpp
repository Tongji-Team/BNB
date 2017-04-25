#include "MainScene.h"
#include "VisibleRect.h"

Scene* MainScene::createScene()
{
	auto scene = Scene::createWithPhysics();
	auto layer = MainScene::create();
	layer->setPhysicsWorld(scene->getPhysicsWorld());
	scene->addChild(layer);
	return scene;
}

bool MainScene::init()
{
	if (!Layer::init())
		return false;

	log("hello mainscene");

	
	addListener();
	addMap();
	

	return true;
}

void MainScene::onEnter()
{
	Layer::onEnter();
	_world->setGravity(Vec2(0, 0));
}

void MainScene::addRole(float x,float y)
{
	_player = Player::create();
	_player->setPosition(Vec2(x,y));
	this->addChild(_player,100);
}

void MainScene::addListener()
{
	_listener_key = EventListenerKeyboard::create();
	_listener_key->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event){
		switch ((int)keyCode)
		{
		case 124:_player->alterLeft();
			break;
		case 127:_player->alterRight();
			break;
		case 142:_player->alterDown();
			break;
		case 146:_player->alterUp();
			break;
		case 59:if (_player->getBombMaxNum() == _player->getBombPresentNum())
				{
					log("full bomb");
				}
				else
				{
					_player->addBomb(_player->getBombPower(), _player->getPosition());
					auto bomb = _player->getBomb();
					this->addChild(bomb);
					DelayTime* delayAction = DelayTime::create(2.0f);
					CallFunc* callFunc = CallFunc::create(CC_CALLBACK_0(Bomb::boom, bomb));
					this->runAction(Sequence::createWithTwoActions(delayAction, callFunc));
				}
				break;
		}

		if ((int)keyCode == 124 || (int)keyCode == 127 || (int)keyCode == 142 || (int)keyCode == 146)
		{
			this->scheduleUpdate();
		}
	};
	_listener_key->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event){
		switch ((int)keyCode)
		{
		case 124:_player->alterLeft();
			break;
		case 127:_player->alterRight();
			break;
		case 142:_player->alterDown();
			break;
		case 146:_player->alterUp();
			break;
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener_key, this);
}

bool MainScene::addMap()
{
	_tileMap = CCTMXTiledMap::create("maps/Tiled_map.tmx");
	if (_tileMap == nullptr)
	{
		log("failed to open the map");
		return false;
	}

	auto origin = Vec2(0.5, 0.5);
	_tileMap->setAnchorPoint(origin);
	_tileMap->setPosition(VisibleRect::center());
	addChild(_tileMap,0);

	TMXObjectGroup* group = _tileMap->getObjectGroup("object");
	ValueMap spawnPoint = group->getObject("player");

	float x = spawnPoint["x"].asFloat();
	float y = spawnPoint["y"].asFloat();
	log("(%f,%f)",x,y);

	addRole(x+380, y+40);//将瓦片地图上的坐标转换为像素点坐标

	return true;
}

void MainScene::update(float dt)
{
	if (_player->getLeft())
		_player->walkLeft();
	if (_player->getRight())
		_player->walkRight();
	if (_player->getUp())
		_player->walkUp();
	if (_player->getDown())
		_player->walkDown();
}