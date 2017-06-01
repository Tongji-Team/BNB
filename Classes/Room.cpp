#include"Room.h"
#include"MainScene.h"
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include "cocostudio/CocoStudio.h"
using namespace cocostudio;
#include "ui/cocosGUI.h"

USING_NS_CC;

bool g_isClient;
std::vector<boost::asio::ip::udp::endpoint> g_clientEndpoint;//用于存放连接的客户端的地址
boost::asio::ip::udp::endpoint g_serverEndpoint;//用于存放服务器的地址
int g_playerID = 1;
int g_mapSeed = 0;
int g_mapName = 0;



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
	auto startItem = MenuItemLabel::create(startLabel, CC_CALLBACK_1(Room::clickStartCallBack, this,_currentMapTag));
	auto startMenu = Menu::create(startItem, nullptr);
	startMenu->setPosition(visibleSize.width*0.8, visibleSize.height/2 - 200);
	this->addChild(startMenu);

	auto findRoomLabel = Label::createWithTTF("Find room", "fonts/Marker Felt.ttf", 40);
	auto findRoomItem = MenuItemLabel::create(findRoomLabel, CC_CALLBACK_1(Room::clickFindRoomCallBack, this,_currentMapTag));
	auto findRoomMenu = Menu::create(findRoomItem, nullptr);
	findRoomMenu->setPosition(visibleSize.width*0.8, visibleSize.height / 2 + 200);
	this->addChild(findRoomMenu);

	auto creatRoomLabel = Label::createWithTTF("Creat room", "fonts/Marker Felt.ttf", 40);
	auto creatRoomItem = MenuItemLabel::create(creatRoomLabel, CC_CALLBACK_1(Room::clickCreatRoomCallBack, this, _currentMapTag));
	auto creatRoomMenu = Menu::create(creatRoomItem, nullptr);
	creatRoomMenu->setPosition(visibleSize.width*0.6, visibleSize.height / 2 + 200);
	this->addChild(creatRoomMenu);

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

void Room::clickSelectedMenuCallBack(Ref* obj,int mapNum)
{
	log("selectedMenu clicked");
	_currentMapTag = mapNum;
	_selectedMap->setString(_mapNames[_currentMapTag - 1]);
	changeMapWindow();
	removeSelectedMenu();
}

void Room::clickStartCallBack(Ref* obj, int mapNum)
{
    log("hello start");

	auto scene = MainScene::createScene();
	Director::getInstance()->replaceScene(scene);

	_threadGroup.interrupt_all();
}
void Room::clickCreatRoomCallBack(Ref* obj,int mapNum)
{
	//预留待补充
	g_isClient = false;
	g_playerID = 1;
	makeMapSeed();

	_threadGroup.create_thread(std::bind(&initBroadcast, this));
	_threadGroup.create_thread(std::bind(&initReceiver, this));
	_threadGroup.create_thread(std::bind(&initClient, this));

	addRomeList();
}

void Room::clickFindRoomCallBack(Ref* obj, int mapNum)
{
	//预留待补充
	g_isClient = true;

	_threadGroup.create_thread(std::bind(&initClient, this));
	addRomeList();
}

void Room::clickRoomListTextCallBack(Ref* obj, ui::ListView* list, int tag)
{
	//信息交互待补充
	log("click room list text");

	for (int i = 9; i < 15; ++i)
	{
		auto listText = dynamic_cast<ui::Text*>(list->getChildByTag(i));
	
		if (i == tag)
		{
			listText->setFontSize(30);
			listText->setTextColor(Color4B::BLACK);
		}
		else
		{
			listText->setFontSize(20);
			listText->setTextColor(Color4B::WHITE);
		}
	}
}
void Room::makeMapSeed()
{
	srand(static_cast<unsigned>(time(NULL)));
	auto seed = rand() % 100 + 1;
	g_mapSeed = seed;
}

void Room::addSelectedMenu()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto mapLabel1 = Label::createWithTTF("map1", "fonts/Marker Felt.ttf", 50);
	auto mapItem1 = MenuItemLabel::create(mapLabel1,CC_CALLBACK_1(Room::clickSelectedMenuCallBack, this, 1));
	auto mapLabel2 = Label::createWithTTF("map2", "fonts/Marker Felt.ttf", 50);
	auto mapItem2 = MenuItemLabel::create(mapLabel2, CC_CALLBACK_1(Room::clickSelectedMenuCallBack, this, 2));

	_selectedMenu = Menu::create(mapItem1, mapItem2,nullptr);
	_selectedMenu->alignItemsVertically();
	_selectedMenu->setPosition(visibleSize.width*0.75, visibleSize.height / 2 - 100);
	addChild(_selectedMenu);
}

void Room::removeSelectedMenu()
{
	removeChild(_selectedMenu);
}

void Room::changeMapWindow()
{
	removeChild(_mapWindow);
	__String* mapName = __String::createWithFormat("image/map%d.png", _currentMapTag);
	_mapWindow = Sprite::create(mapName->getCString());
	auto visibleSize = Director::getInstance()->getVisibleSize();
	_mapWindow->setPosition(visibleSize.width*0.25, visibleSize.height / 2);
	addChild(_mapWindow);
}

void Room::addRomeList()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto node = CSLoader::getInstance()->createNode("cocosstudio/Layer.csb");

	auto list = dynamic_cast<ui::ListView*>(node->getChildByName("ListView_1"));

	auto listImage = dynamic_cast<ui::ImageView*>(node->getChildByName("backImage"));
	listImage->loadTexture("image/bg-st.png");

	auto startButton = dynamic_cast<ui::Button*>(node->getChildByName("startButton"));
	startButton->addTouchEventListener(CC_CALLBACK_1(Room::clickStartCallBack, this, _currentMapTag));
	startButton->setTitleFontSize(30);
	for (int tag = 9; tag < 15; ++tag)
	{
		auto listText = dynamic_cast<ui::Text*>(list->getChildByTag(tag));
		listText->addTouchEventListener(CC_CALLBACK_1(Room::clickRoomListTextCallBack, this, list, tag));
	}

	__String* mapName = __String::createWithFormat("image/map%d.png", _currentMapTag);
	auto mapWindow = Sprite::create(mapName->getCString());
	mapWindow->setPosition(visibleSize.width*0.7,visibleSize.height*0.6);
	node->addChild(mapWindow);

	addChild(node, 1);
}

void Room::initBroadcast(Room* ptr)
{
	namespace ip = boost::asio::ip;
	boost::asio::io_service io_service;

	ip::udp::socket socket(io_service, ip::udp::endpoint(ip::udp::v4(), 0));
	socket.set_option(boost::asio::socket_base::broadcast(true));
	ip::udp::endpoint broadcast_endpoint(ip::address_v4::broadcast(), 6001);

	char buf[50];
	int icount = 1;
	while (ptr->_clientNum < 4)
	{
		sprintf(buf, "Message: %d, player: %d, map: %d, mapSeed: %d", icount, ptr->_clientNum, ptr->_currentMapTag,g_mapSeed);
		++icount;
		socket.send_to(boost::asio::buffer(buf, strlen(buf) + 1), broadcast_endpoint);
		boost::this_thread::sleep(boost::posix_time::seconds(2));
	}
	socket.close();
}

void Room::initReceiver(Room* ptr)
{
	namespace ip = boost::asio::ip;
	boost::asio::io_service io_service;

	ip::udp::socket socket(io_service, ip::udp::endpoint(ip::udp::v4(), 6003));
	ip::udp::endpoint sender_endpoint;

	char buf[50];
	while (ptr->_clientNum < 5)
	{
		socket.receive_from(boost::asio::buffer(buf), sender_endpoint);
		ip::udp::endpoint client_point(sender_endpoint.address(), 6001);
		g_clientEndpoint.push_back(sender_endpoint);
		ptr->_clientNum++;

		char sendBuf[50];
		sprintf(sendBuf, "successfully connected! player%d", ptr->_clientNum);
		socket.send_to(boost::asio::buffer(sendBuf, strlen(sendBuf) + 1), client_point);
		sprintf(sendBuf, "%d", g_mapSeed);
		socket.send_to(boost::asio::buffer(sendBuf, strlen(sendBuf) + 1), client_point);

		boost::this_thread::sleep(boost::posix_time::seconds(1));
		log("reveiver running");
	}

	socket.close();
}

void Room::initClient(Room* ptr)
{
	namespace ip = boost::asio::ip;
	boost::asio::io_service io_service;

	bool connected = false;

	ip::udp::socket socket(io_service, ip::udp::endpoint(ip::udp::v4(), 6001));
	ip::udp::endpoint sender_endpoint;

	char buf[50];
	while (1)
	{
		std::size_t bytes_transferred = socket.receive_from(boost::asio::buffer(buf), sender_endpoint);
		log("got %d bytes.", bytes_transferred);
		log("the message: %s", buf);
		g_serverEndpoint = sender_endpoint;

		std::string checkStr(buf);
		if (checkStr.find("success") != std::string::npos)
		{
			if (checkStr.rfind("player1") != std::string::npos)
				g_playerID = 1;
			else if (checkStr.rfind("player2") != std::string::npos)
				g_playerID = 2;
			else if (checkStr.rfind("player3") != std::string::npos)
				g_playerID = 3;
			else
				g_playerID = 4;
		}

		if (strtol(buf, NULL, 10) != 0)
		{
			g_mapSeed= static_cast<int>(strtol(buf, NULL, 10));
			log("deal message: %d",g_mapSeed);
			return;
		}

		if (!connected)
		{
			char *message = "connect";
			ip::udp::socket sender(io_service, ip::udp::endpoint(ip::udp::v4(), 6005));
			ip::udp::endpoint server_point(sender_endpoint.address(), 6003);
			sender.send_to(boost::asio::buffer(message, strlen(message) + 1), server_point);
			sender.close();
			connected = true;
		}

		boost::this_thread::sleep(boost::posix_time::seconds(1));
		log("client running");
	}
	
	socket.close();
}

