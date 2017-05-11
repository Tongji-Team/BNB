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
	addMap();//里面包含添加角色的功能
	

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
	_player->setAnchorPoint(Vec2(0.5,0.5));
	_player->setScale(0.4);
	_player->setPosition(Vec2(x, y));
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
					_player->addBomb(_player->getBombPower(), getBombPosition(tileCoordFromPosition(_player->getPosition())));
					auto bomb = _player->getBomb();
					bomb->setScale(0.4);
					this->addChild(bomb);
					DelayTime* delayAction = DelayTime::create(2.0f);
					CallFunc* callFuncRemove = CallFunc::create(CC_CALLBACK_0(MainScene::removeBlock, this, tileCoordFromPosition(bomb->getPosition())));
					CallFunc* callFuncBomb = CallFunc::create(CC_CALLBACK_0(Bomb::boom, bomb));
					this->runAction(Sequence::create(delayAction, callFuncRemove, callFuncBomb, NULL));
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

Point MainScene::getBombPosition(Point coord)
{
	_mapProp[coord.x][coord.y] = 1;
	return this->_mapCoord[coord.x][coord.y];
}

bool MainScene::addMap()
{
	_tileMap = CCTMXTiledMap::create("maps/Tiled_map.tmx");
	if (_tileMap == nullptr)
	{
		log("failed to open the map");
		return false;
	}

	auto origin = Vec2(0.5,0.5);
	_tileMap->setAnchorPoint(origin);
	_tileMap->setPosition(VisibleRect::center());
	log("center:%f,%f", VisibleRect::center().x, VisibleRect::center().y);
	addChild(_tileMap,0);

	TMXObjectGroup* group = _tileMap->getObjectGroup("object");
	ValueMap spawnPoint = group->getObject("player");

	auto x = spawnPoint["x"].asFloat();
	auto y = spawnPoint["y"].asFloat();
	log("PlayerPoint:%f,%f", x, y);
	log("height:%f", _tileMap->getMapSize().height);

	addRole(x + 360, y + 80);//将瓦片地图上的坐标转换为像素点坐标

	_collidable = _tileMap->getLayer("collision");
	auto item = _tileMap->getLayer("item");
	auto road = _tileMap->getLayer("road");

	for (int i = 0; i < 15; ++i)
	{
		std::vector<Vec2> map;
		std::vector<int> prop;
		for (int j = 0; j < 15; ++j)
		{
			map.push_back(road->getPositionAt(Vec2(i, j)) + Vec2(340 + 20, 60 + 20));
			log("%f,%f", map[j].x, map[j].y);

			auto tileGidCol = _collidable->getTileGIDAt(Point(i,j));
			auto tileGidIte = item->getTileGIDAt(Point(i, j));

			if (tileGidCol > 0)
				prop.push_back(1);//1代表该块为碰撞块

			else if (tileGidIte > 0)
				prop.push_back(2);//2代表该块为道具块

			else
				prop.push_back(0);//0代表无障碍物
		}
		_mapCoord.push_back(map);
		_mapProp.push_back(prop);
	}

	return true;
}

void MainScene::update(float dt)
{
	auto pos = _player->getPosition();
	if (_player->getLeft())
	{
		pos = _player->walkLeft();
		if (this->checkCollidable(pos-Vec2(16,16))||this->checkCollidable(pos-Vec2(16,-16)))//检查左下和左上
		{
			log("collided");
		}
		else
		{
			_player->setPosition(pos);
		}
	}
	if (_player->getRight())
	{
		pos = _player->walkRight();
		if (this->checkCollidable(pos+Vec2(16,16))||this->checkCollidable(pos+Vec2(16,-16)))//检查右下和右上
		{
			log("collided");
		}
		else
		{
			_player->setPosition(pos);
		}
	}
	if (_player->getUp())
	{
		pos = _player->walkUp();
		if (this->checkCollidable(pos+Vec2(16,16))||this->checkCollidable(pos+Vec2(-16,16)))//检查左上和右上
		{
			log("collided");
		}
		else
		{
			_player->setPosition(pos);
		}
	}
	if (_player->getDown())
	{
		pos = _player->walkDown();
		if (this->checkCollidable(pos-Vec2(16,16))||this->checkCollidable(pos-Vec2(-16,16)))//检查左下和右下
		{
			log("collided");
		}
		else
		{
			_player->setPosition(pos);
		}
	}
}

Point MainScene::tileCoordFromPosition(Point pos)
{
	int x = (pos.x - 340) / 40;
	int y = (660 - pos.y) / 40;
	return Point(x, y);
}

bool MainScene::checkCollidable(Point pos)
{
	log("%f,%f", pos.x, pos.y);
	Point tileCoord = this->tileCoordFromPosition(pos);
	return _mapProp[tileCoord.x][tileCoord.y] != 0;
}

void MainScene::removeBlock(Point coord)
{
	log("remove:%f,%f", coord.x, coord.y);
	_mapProp[coord.x][coord.y] = 0;
}