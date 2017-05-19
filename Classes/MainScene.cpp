#include "MainScene.h"
#include "VisibleRect.h"
#include "Item.h"
#include <cstdlib>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

extern bool g_isClient;
extern int g_playerID;
extern std::vector<boost::asio::ip::udp::endpoint> g_clientEndpoint;//用于存放连接的客户端的地址
extern boost::asio::ip::udp::endpoint g_serverEndpoint;//用于存放服务器的地址
extern int g_mapSeed;

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
	addPlayer();

	if (g_isClient)//作为客户端运行
	{
		log("I am a client");
		_threadGroup.create_thread(std::bind(&initClientSend, this));
		_threadGroup.create_thread(std::bind(&initClientReceive, this));
	}
	else//作为客户端和服务端运行
	{
		log("I am a server");
		_threadGroup.create_thread(std::bind(&initServer, this));
		//_threadGroup.create_thread(std::bind(&initClientReceive, this));//此处是为了进行测试
		_threadGroup.create_thread(std::bind(&initClientSend, this));
	}
	addItem();

	return true;
}

void MainScene::onEnter()
{
	Layer::onEnter();
	_world->setGravity(Vec2(0, 0));
}

Player* MainScene::addRole(float x,float y)
{
	auto player = Player::create();
	player->setAnchorPoint(Vec2(0.5,0.5));
	player->setScale(1);
	player->setPosition(Vec2(x, y));
	this->addChild(player,100);
	return player;
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

	};

	this->scheduleUpdate();

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

void MainScene::addPlayer()
{
	TMXObjectGroup* group = _tileMap->getObjectGroup("object");
	int icount = 1;
	while (icount < 5)
	{
		char playerName[10];
		sprintf(playerName, "player%d", icount);
		ValueMap spawnPoint = group->getObject(playerName);

		auto x = spawnPoint["x"].asFloat();
		auto y = spawnPoint["y"].asFloat();
		log("PlayerPoint:%f,%f", x, y);
		log("height:%f", _tileMap->getMapSize().height);

		_playerGroup.push_back(addRole(x + 360, y + 80));
		++icount;
	}

	_player = _playerGroup[g_playerID - 1];

}

void MainScene::addItem()
{
	srand(g_mapSeed);
	for (auto flag = 1; flag < 4;)//对应三种道具
	{
		auto i = rand() % 15;
		auto j = rand() % 15;

		if (_mapProp[i][j] == 0)
		{
			auto item = Item::create(flag);
			item->setAnchorPoint(Point(0.5, 0.5));
			item->setPosition(_mapCoord[i][j]);
			item->setTag(i * 100 + j);
			addChild(item, 50);
		    _mapProp[i][j] = 10 + flag;
			++flag;
		}
	}
}

void MainScene::update(float dt)
{
	for (auto checkPlayer : _playerGroup)
	{
		auto pos = checkPlayer->getPosition();
		if (checkPlayer->getLeft())
		{
			pos = checkPlayer->walkLeft();
			if (this->checkCollidable(pos - Vec2(16, 16), checkPlayer) || this->checkCollidable(pos - Vec2(16, -16), checkPlayer))//检查左下和左上
			{
				log("collided");
			}
			else
			{
				checkPlayer->setPosition(pos);
			}
		}
		if (checkPlayer->getRight())
		{
			pos = checkPlayer->walkRight();
			if (this->checkCollidable(pos + Vec2(16, 16), checkPlayer) || this->checkCollidable(pos + Vec2(16, -16), checkPlayer))//检查右下和右上
			{
				log("collided");
			}
			else
			{
				checkPlayer->setPosition(pos);
			}
		}
		if (checkPlayer->getUp())
		{
			pos = checkPlayer->walkUp();
			if (this->checkCollidable(pos + Vec2(16, 16), checkPlayer) || this->checkCollidable(pos + Vec2(-16, 16), checkPlayer))//检查左上和右上
			{
				log("collided");
			}
			else
			{
				checkPlayer->setPosition(pos);
			}
		}
		if (checkPlayer->getDown())
		{
			pos = checkPlayer->walkDown();
			if (this->checkCollidable(pos - Vec2(16, 16), checkPlayer) || this->checkCollidable(pos - Vec2(-16, 16), checkPlayer))//检查左下和右下
			{
				log("collided");
			}
			else
			{
				checkPlayer->setPosition(pos);
			}
		}
	}
}

Point MainScene::tileCoordFromPosition(Point pos)
{
	int x = (pos.x - 340) / 40;
	int y = (660 - pos.y) / 40;
	return Point(x, y);
}

bool MainScene::checkCollidable(Point pos, Player* ptr)
{
	log("%f,%f", pos.x, pos.y);
	Point tileCoord = this->tileCoordFromPosition(pos);

	if (_mapProp[tileCoord.x][tileCoord.y] > 10)
	{
		if (ptr->eatItem(_mapProp[tileCoord.x][tileCoord.y] - 10))
		{
			_mapProp[tileCoord.x][tileCoord.y] = 0;
			auto item = getChildByTag(tileCoord.x * 100 + tileCoord.y);
			item->removeFromParentAndCleanup(true);
			return false;
		}
	}

	return _mapProp[tileCoord.x][tileCoord.y] != 0;
}

void MainScene::removeBlock(Point coord)
{
	log("remove:%f,%f", coord.x, coord.y);
	_mapProp[coord.x][coord.y] = 0;
}

void MainScene::dealMessage(char* Buf, MainScene*ptr)
{
	/*示例消息：p2 u l pos 123.45,123.45*/
	std::string checkStr(Buf);
	Player* checkPlayer;

	if (checkStr.find("p1") != std::string::npos)
		checkPlayer = ptr->_playerGroup[0];
	else if (checkStr.find("p2") != std::string::npos)
		checkPlayer = ptr->_playerGroup[1];
	else if (checkStr.find("p3") != std::string::npos)
		checkPlayer = ptr->_playerGroup[2];
	else
		checkPlayer = ptr->_playerGroup[3];

	if (checkStr.find("u") != std::string::npos)
		checkPlayer->_up = true;
	else
		checkPlayer->_up = false;
	if (checkStr.find("d") != std::string::npos)
		checkPlayer->_down = true;
	else
		checkPlayer->_down = false;
	if (checkStr.find("l") != std::string::npos)
		checkPlayer->_left = true;
	else
		checkPlayer->_left = false;
	if (checkStr.find("r") != std::string::npos)
		checkPlayer->_right = true;
	else
		checkPlayer->_right = false;

}

void MainScene::initServer(MainScene* ptr)
{
	namespace ip = boost::asio::ip;
	boost::asio::io_service io_service;

	ip::udp::socket socket(io_service, ip::udp::endpoint(ip::udp::v4(), 6105));
	ip::udp::endpoint sender_endpoint;

	char buf[80];
	while (1)
	{
		socket.receive_from(boost::asio::buffer(buf), sender_endpoint);
		log("receive message: %s",buf);
		dealMessage(buf, ptr);

		for (auto it : g_clientEndpoint)
		{
			if (it.address() != sender_endpoint.address())//此处无效果
			{
				ip::udp::endpoint client_point(it.address(), 6104);
				socket.send_to(boost::asio::buffer(buf, strlen(buf) + 1), client_point);
			}
		}
	}
	socket.close();
}

void MainScene::initClientSend(MainScene* ptr)
{
	namespace ip = boost::asio::ip;
	boost::asio::io_service io_service;

	ip::udp::socket socket(io_service, ip::udp::endpoint(ip::udp::v4(), 6106));
	ip::udp::endpoint server_point(g_serverEndpoint.address(), 6105);

	auto left = ptr->_player->_left;
	auto right = ptr->_player->_right;
	auto up = ptr->_player->_up;
	auto down = ptr->_player->_down;

	char buf[80];
	while (1)
	{
		if (left != ptr->_player->_left || right != ptr->_player->_right || up != ptr->_player->_up || down != ptr->_player->_down)
		{
			Vec2 pos = ptr->_player->getPosition();
			sprintf(buf, "p%d", g_playerID);

			if (ptr->_player->_left == true)
				sprintf(buf + strlen(buf), " l");
			if (ptr->_player->_right == true)
				sprintf(buf + strlen(buf), " r");
			if (ptr->_player->_up == true)
				sprintf(buf + strlen(buf), " u");
			if (ptr->_player->_down == true)
				sprintf(buf + strlen(buf), " d");

			sprintf(buf + strlen(buf), " pos %.2f,%.2f ", pos.x, pos.y);

			socket.send_to(boost::asio::buffer(buf, strlen(buf) + 1), server_point);

			left = ptr->_player->_left;
			right = ptr->_player->_right;
			up = ptr->_player->_up;
			down = ptr->_player->_down;
		}
	}
	socket.close();
}

void MainScene::initClientReceive(MainScene*ptr)
{
	namespace ip = boost::asio::ip;
	boost::asio::io_service io_service;

	ip::udp::socket socket(io_service, ip::udp::endpoint(ip::udp::v4(), 6104));
	ip::udp::endpoint server_point;

	char buf[80];
	while (1)
	{
		socket.receive_from(boost::asio::buffer(buf), server_point);
		dealMessage(buf, ptr);
	}

	socket.close();
}