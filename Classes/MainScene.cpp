#include "MainScene.h"
#include "VisibleRect.h"
#include "Item.h"
#include <cstdlib>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

extern bool g_isClient;
extern int g_playerID;
extern std::vector<boost::asio::ip::udp::endpoint> g_clientEndpoint;//用于存放连接的客户端的地址
extern boost::asio::ip::udp::endpoint g_serverEndpoint;//用于存放服务器的地址
extern int g_mapSeed;

#define deltaX 240
#define deltaY 560
#define tileSize 32

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
		_threadGroup.create_thread(std::bind(&initClientReceive, this));//此处是为了进行测试
		_threadGroup.create_thread(std::bind(&initClientSend, this));
	}
	addItem();

	return true;
}

void MainScene::onEnter()
{
	Layer::onEnter();
	_world->setGravity(Vec2(0, 0));
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/music.wav", true);
}

Player* MainScene::addRole(float x,float y)
{
	auto player = Player::create();
	player->setAnchorPoint(Vec2(0.5,0.5));
	player->setPosition(Vec2(x, y));
	player->setName("player");
	player->setScale(0.8);
	this->addChild(player,100);
	player->setGlobalZOrder(100);
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
					log("full bomb");
				else
					placeBomb(_player);
				break;
		}

	};

	_listener_contact = EventListenerPhysicsContact::create();
	_listener_contact->onContactBegin = CC_CALLBACK_1(MainScene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithFixedPriority(_listener_contact, 1);

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
	Size size = _tileMap->getContentSize();
	log("map size: %f,%f", size.height, size.width);
	addChild(_tileMap,0);

	auto collidable = _tileMap->getLayer("collision");
	_item = _tileMap->getLayer("item");
	_item->setGlobalZOrder(50);
	auto road = _tileMap->getLayer("road");

	for (int i = 0; i < 15; ++i)
	{
		std::vector<Vec2> map;
		std::vector<int> prop;
		for (int j = 0; j < 15; ++j)
		{
			map.push_back(road->getPositionAt(Vec2(i, j)) + Vec2(deltaX + 16, deltaY - 480 + 16));

			auto tileGidCol = collidable->getTileGIDAt(Point(i,j));
			auto tileGidIte = _item->getTileGIDAt(Point(i, j));

			if (tileGidCol > 0)
				prop.push_back(1);//1代表该块为碰撞块

			else if (tileGidIte > 0)
				prop.push_back(4);//4代表该块为道具块

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
		
		auto player = addRole(x + deltaX + 16, y + deltaY - 480 + 16);
		_playerGroup.pushBack(player);
		++icount;
	}

	_player = _playerGroup.at(g_playerID - 1);

}

void MainScene::addItem()
{
	srand(g_mapSeed);
	for (auto flag = 1; flag < 4;)//对应三种道具
	{
		auto i = rand() % 15;
		auto j = rand() % 15;

		if (_mapProp[i][j] == 4)
		{
			auto item = Item::create(flag);
			item->setAnchorPoint(Point(0.5, 0.5));
			item->setPosition(_mapCoord[i][j]);
			item->setTag(i * 100 + j);
			addChild(item, 0);
		    _mapProp[i][j] += (10 + flag);
			++flag;
		}
	}
}

void MainScene::placeBomb(Player* player)
{
	player->addBomb(player->getBombPower(), getBombPosition(tileCoordFromPosition(player->getPosition())));
	auto bomb = player->getBomb();
	bomb->setScale(0.4);
	this->addChild(bomb);
	player->_addBomb = true;
	auto pos = tileCoordFromPosition(bomb->getPosition());
	DelayTime* delayAction = DelayTime::create(2.0f);
	CallFunc* callFuncRemove = CallFunc::create(CC_CALLBACK_0(MainScene::removeBlock, this, pos));
	CallFunc* callFuncBomb = CallFunc::create(CC_CALLBACK_0(Bomb::boom, bomb, this, pos));
	this->runAction(Sequence::create(delayAction, callFuncRemove, callFuncBomb, NULL));
}

void MainScene::update(float dt)
{
	for (Vector<Player*>::iterator it = _playerGroup.begin(); it != _playerGroup.end();)
	{
		if (_playerGroup.size() == 1)
			break;

		if (!(*it)->_isAlive)
		{
			if (*it != _player)
				(*it)->removeFromParent();
			it = _playerGroup.erase(it);
			continue;
		}

		auto checkPlayer = *it;
		++it;
		auto pos = checkPlayer->getPosition();
		
		if (checkPlayer->getLeft())//向左移动并播放动画
		{
			pos = checkPlayer->walkLeft();
			if (this->checkCollidable(pos - Vec2(16, 16), checkPlayer) || this->checkCollidable(pos - Vec2(16, -16), checkPlayer))//检查左下和左上
			{
				log("collided");
			}
			else
			{
				checkPlayer->setPosition(pos);
				if (!checkPlayer->_goLeft)
				{
					checkPlayer->runAction(checkPlayer->_animateLeft);
					checkPlayer->_goLeft = true;
				}
			}
		}
		else
		{
			if (checkPlayer->_goLeft)
			{
				checkPlayer->stopAction(checkPlayer->_animateLeft);
				checkPlayer->_goLeft = false;
			}
		}

		if (checkPlayer->getRight())//向右移动并播放动画
		{
			pos = checkPlayer->walkRight();
			if (this->checkCollidable(pos + Vec2(16, 16), checkPlayer) || this->checkCollidable(pos + Vec2(16, -16), checkPlayer))//检查右下和右上
			{
				log("collided");
			}
			else
			{
				checkPlayer->setPosition(pos);
				if (!checkPlayer->_goRight)
				{
					checkPlayer->runAction(checkPlayer->_animateRight);
					checkPlayer->_goRight = true;
				}
			}
		}
		else
		{
			if (checkPlayer->_goRight)
			{
				checkPlayer->stopAction(checkPlayer->_animateRight);
				checkPlayer->_goRight = false;
			}
		}

		if (checkPlayer->getUp())//向上移动并播放动画
		{
			pos = checkPlayer->walkUp();
			if (this->checkCollidable(pos + Vec2(16, 16), checkPlayer) || this->checkCollidable(pos + Vec2(-16, 16), checkPlayer))//检查左上和右上
			{
				log("collided");
			}
			else
			{
				checkPlayer->setPosition(pos);
				if (!checkPlayer->_goUp)
				{
					checkPlayer->runAction(checkPlayer->_animateUp);
					checkPlayer->_goUp = true;
				}
			}
		}
		else
		{
			if (checkPlayer->_goUp)
			{
				checkPlayer->stopAction(checkPlayer->_animateUp);
				checkPlayer->_goUp = false;
			}
		}

		if (checkPlayer->getDown())//向右移动并播放动画
		{
			pos = checkPlayer->walkDown();
			if (this->checkCollidable(pos - Vec2(16, 16), checkPlayer) || this->checkCollidable(pos - Vec2(-16, 16), checkPlayer))//检查左下和右下
			{
				log("collided");
			}
			else
			{
				checkPlayer->setPosition(pos);
				if (!checkPlayer->_goDown)
				{
					checkPlayer->runAction(checkPlayer->_animateDown);
					checkPlayer->_goDown = true;
				}
			}
		}
		else
		{
			if (checkPlayer->_goDown)
			{
				checkPlayer->stopAction(checkPlayer->_animateDown);
				checkPlayer->_goDown = false;
			}
		}
	}
}

Point MainScene::tileCoordFromPosition(Point pos)
{
	int x = (pos.x - deltaX) / tileSize;
	int y = (deltaY - pos.y) / tileSize;
	return Point(x, y);
}

bool MainScene::checkCollidable(Point pos, Player* ptr)
{
	Point tileCoord = this->tileCoordFromPosition(pos);

	if (_mapProp[tileCoord.x][tileCoord.y] > 10 && _mapProp[tileCoord.x][tileCoord.y] < 14)
	{
		if (ptr->eatItem(_mapProp[tileCoord.x][tileCoord.y] - 10))
		{
			_mapProp[tileCoord.x][tileCoord.y] = 0;
			auto item = getChildByTag(tileCoord.x * 100 + tileCoord.y);
			item->removeFromParentAndCleanup(true);
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/get.wav", false);
			return false;
		}
	}

	return _mapProp[tileCoord.x][tileCoord.y] != 0;
}

bool MainScene::onContactBegin(const PhysicsContact& contact)
{
	auto objA = static_cast<Sprite*>(contact.getShapeA()->getBody()->getNode());
	auto objB = static_cast<Sprite*>(contact.getShapeB()->getBody()->getNode());
	auto nameA = objA->getName();
	auto nameB = objB->getName();

	if (nameA == "player"&&nameB == "bomb")
	{
		log("player has been slayed");
		auto player = static_cast<Player*>(objA);
		player->_isAlive = false;
	}

	else if (nameA == "bomb"&&nameB == "player")
	{
		log("player has been slayed");
		auto player = static_cast<Player*>(objB);
		player->_isAlive = false;
	}

	return true;
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
		checkPlayer = ptr->_playerGroup.at(0);
	else if (checkStr.find("p2") != std::string::npos)
		checkPlayer = ptr->_playerGroup.at(1);
	else if (checkStr.find("p3") != std::string::npos)
		checkPlayer = ptr->_playerGroup.at(2);
	else
		checkPlayer = ptr->_playerGroup.at(3);

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
	if (checkStr.find("b") != std::string::npos)//add bomb
		ptr->placeBomb(checkPlayer);

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

		for (auto it : g_clientEndpoint)
		{
			if (it.address() != sender_endpoint.address())
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
	while (ptr->_player->_isAlive)
	{
		if (left != ptr->_player->_left || right != ptr->_player->_right || up != ptr->_player->_up || down != ptr->_player->_down
			|| ptr->_player->_addBomb)
		{
			Vec2 pos = ptr->_player->getPosition();
			sprintf(buf, "p%d", g_playerID);

			if (ptr->_player->_left)
				sprintf(buf + strlen(buf), " l");
			if (ptr->_player->_right)
				sprintf(buf + strlen(buf), " r");
			if (ptr->_player->_up)
				sprintf(buf + strlen(buf), " u");
			if (ptr->_player->_down)
				sprintf(buf + strlen(buf), " d");
			if (ptr->_player->_addBomb)
			{
				sprintf(buf + strlen(buf), " b");
				ptr->_player->_addBomb = false;
			}

			sprintf(buf + strlen(buf), " pos %.2f,%.2f ", pos.x, pos.y);

			socket.send_to(boost::asio::buffer(buf, strlen(buf) + 1), server_point);

			left = ptr->_player->_left;
			right = ptr->_player->_right;
			up = ptr->_player->_up;
			down = ptr->_player->_down;
		}
	}
	boost::this_thread::sleep(boost::posix_time::seconds(2));
	ptr->_player->removeFromParent();
	ptr->_player = NULL;
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
