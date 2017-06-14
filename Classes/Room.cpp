#include"Room.h"
#include"MainScene.h"
#include"StartScene.h"
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include "cocostudio/CocoStudio.h"
#include "ui/cocosGUI.h"

using namespace cocostudio;
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

	auto bg = Sprite::create("image/roomBack.png");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(bg);

	_mapWindow = Sprite::create("image/map0.png");
	_mapWindow->setPosition(visibleSize.width*0.25, visibleSize.height*0.7);
	this->addChild(_mapWindow);

	_selectedMap = Label::createWithTTF("choose your map", "fonts/Marker Felt.ttf", 50);
	auto initMenuItem = MenuItemLabel::create(_selectedMap, CC_CALLBACK_1(Room::clickInitMenuCallBack, this));

	_initMenu = Menu::create(initMenuItem, nullptr);
	_initMenu->setPosition(visibleSize.width*0.25, visibleSize.height*0.4);
	this->addChild(_initMenu);

	auto backLabel = Label::createWithTTF("Back", "fonts/Marker Felt.ttf", 40);
	auto backItem = MenuItemLabel::create(backLabel, CC_CALLBACK_1(Room::clickRoomBackCallBack, this));
	auto backMenu = Menu::create(backItem, nullptr);
	backMenu->setPosition(visibleSize.width*0.75, visibleSize.height*0.3);
	this->addChild(backMenu);

	auto findRoomLabel = Label::createWithTTF("Find room", "fonts/Marker Felt.ttf", 40);
	auto findRoomItem = MenuItemLabel::create(findRoomLabel, CC_CALLBACK_1(Room::clickFindRoomCallBack, this, _currentMapTag));
	auto findRoomMenu = Menu::create(findRoomItem, nullptr);
	findRoomMenu->setPosition(visibleSize.width*0.75, visibleSize.height*0.5);
	this->addChild(findRoomMenu);

	auto creatRoomLabel = Label::createWithTTF("Create room", "fonts/Marker Felt.ttf", 40);
	auto creatRoomItem = MenuItemLabel::create(creatRoomLabel, CC_CALLBACK_1(Room::clickCreatRoomCallBack, this, _currentMapTag));
	auto creatRoomMenu = Menu::create(creatRoomItem, nullptr);
	creatRoomMenu->setPosition(visibleSize.width*0.75, visibleSize.height*0.7);
	this->addChild(creatRoomMenu);

	addMapNames();

	return true;
}

//void Room::onEnter()

void Room::clickInitMenuCallBack(Ref* obj)
{
	log("initMenu clicked");
	if (!mapMenuIsShow)
	{
		addSelectedMenu();
		mapMenuIsShow = 1;
	}
	else
	{
		removeSelectedMenu();
		mapMenuIsShow = 0;
	}
}

void Room::clickSelectedMenuCallBack(Ref* obj, int mapNum)
{
	log("selectedMenu clicked");
	_currentMapTag = mapNum;
	_selectedMap->setString(_mapNames[_currentMapTag - 1]);
	changeMapWindow();
	removeSelectedMenu();
	mapMenuIsShow = 0;
}

void Room::clickStartCallBack(Ref* obj, int mapNum)
{
	if (_clicked)
		return;
	log("hello start");

	_clicked = true;
	_broadRunning = false;
	_threadGroup.interrupt_all();

	auto scene = MainScene::createScene();
	Director::getInstance()->replaceScene(scene);
}
void Room::clickCreatRoomCallBack(Ref* obj, int mapNum)
{
	addSetRoomNameLayer();
}

void Room::clickFindRoomCallBack(Ref* obj, int mapNum)
{
	//预留待补充
	g_isClient = true;

	if (!this->_clientRunning)
	{
		this->_clientRunning = true;
		_threadGroup.create_thread(std::bind(&initClient, this));
	}
	addRomeList();
}

void Room::clickRoomListTextCallBack(Ref* obj, ui::ListView* list, int tag)
{
	//信息交互待补充
	log("click room list text");
	for (int i = 10; i < 15; ++i)
	{
		auto listText = dynamic_cast<ui::Text*>(list->getChildByTag(i));

		if (i == tag)
		{
			std::string title = listText->getString(); 
			_chosenRoom = std::string(title, 0, title.find(' ')); //设置被选中的房间
			log("%s", _chosenRoom.c_str());
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

void Room::clickRoomBackCallBack(Ref* obj)
{
	log("click back");
	auto scene = StartScene::createScene();
	auto reScene = TransitionTurnOffTiles::create(1.0f, scene);
	if (_isReceiving)
		sendStop();
	Director::getInstance()->replaceScene(reScene);
}

void Room::clickRoomListBackCallBack(Ref* obj)
{
	removeRoomList();
}

void Room::clickRoomListRefreshCallBack(Ref* obj)
{
	removeRoomList();
	addRomeList();
}

void Room::clickSetRoomOkCallBack(Ref* obj,ui::TextField* inputField)
{
	auto name = inputField->getString();
	if (name != "")
	{
		_myname = name;
		_rooms.pushBack(RoomItem(name, 1));  //向_rooms中添加新建的房间

		g_isClient = false;
		g_playerID = 1;
		makeMapSeed();
		_isOwner = true;
		_clientNum = 0;

		//待补充：间新建房间的信息传递给其他服务器/客户端
		_threadGroup.create_thread(std::bind(&initBroadcast, this));
		if (!this->_isReceiving)
		{
			this->_isReceiving = true;
			_threadGroup.create_thread(std::bind(&initReceiver, this));
		}
		if (!this->_clientRunning)
		{
			this->_clientRunning = true;
			_threadGroup.create_thread(std::bind(&initClient, this));
		}


		removeSetRoomNameLayer();
		addReadyRoomLayer(name);
	}
}

void Room::clickSetRoomInputCallBack(Ref* obj, ui::TextField* inputField)
{
	inputField->setPlaceHolderColor(Color3B::WHITE);
	inputField->setPlaceHolder(" ");
}

void Room::clickSetRoomBackCallBack(Ref* obj)
{
	removeSetRoomNameLayer();
	//_threadGroup.interrupt_all();
}

void Room::clickJoinCallBack(Ref* obj, std::string& roomName)
{
	
	auto room = _rooms.getRoomByName(_chosenRoom); 
	if (room != _rooms.end())
	{
		int playerNum = room->getPlayerNum() + 1;
		room->setPlayerNum(playerNum); //改变房间中玩家数量信息
		_joinRoom = true;
		
		//待补充：将房间玩家数量改变的信息传给其他服务器/客户端

		removeRoomList();
		addReadyRoomLayer(_chosenRoom);
	}
	else
	{
		//
	}	

}

void Room::clickReadyRoomBackCallBack(Ref* obj)
{
	removeReadyRoomLayer();
	this->_broadRunning = false;
}

void Room::makeMapSeed()
{
	srand(static_cast<unsigned>(time(NULL)));
	auto seed = rand() % 100 + 1;
	g_mapSeed = seed;
}

void Room::addMapNames()
{
	_mapNames.push_back("        map1");
	_mapNames.push_back("        map2");
}

void Room::addSelectedMenu()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto mapLabel1 = Label::createWithTTF("map1", "fonts/Marker Felt.ttf", 50);
	auto mapItem1 = MenuItemLabel::create(mapLabel1, CC_CALLBACK_1(Room::clickSelectedMenuCallBack, this, 1));
	auto mapLabel2 = Label::createWithTTF("map2", "fonts/Marker Felt.ttf", 50);
	auto mapItem2 = MenuItemLabel::create(mapLabel2, CC_CALLBACK_1(Room::clickSelectedMenuCallBack, this, 2));

	_selectedMenu = Menu::create(mapItem1, mapItem2, nullptr);
	_selectedMenu->alignItemsVertically();
	_selectedMenu->setPosition(visibleSize.width*0.25, visibleSize.height*0.25);
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
	_mapWindow->setPosition(visibleSize.width*0.25, visibleSize.height*0.7);
	addChild(_mapWindow);
}

void Room::addRomeList()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//设置虚化背景
	_backColor = LayerColor::create();
	_backColor->initWithColor(Color4B(162, 162, 162, 128));
	//获取控件
	_roomListLayer = CSLoader::getInstance()->createNode("cocosstudio/roomListLayer.csb");
	auto roomListBack = dynamic_cast<ui::ImageView*>(_roomListLayer->getChildByName("roomListBack"));
	auto roomListMenu = dynamic_cast<ui::ListView*>(roomListBack->getChildByName("roomListMenu"));
	auto joinButton = dynamic_cast<ui::Button*>(roomListBack->getChildByName("joinButton"));
	auto backButton = dynamic_cast<ui::Button*>(roomListBack->getChildByName("backButton"));
	auto refreshButton = dynamic_cast<ui::Button*>(roomListBack->getChildByName("refreshButton"));
	for (int tag = 10; tag < 15; ++tag)
	{
		auto listText = dynamic_cast<ui::Text*>(roomListMenu->getChildByTag(tag));
		listText->addTouchEventListener(CC_CALLBACK_1(Room::clickRoomListTextCallBack, this, roomListMenu, tag));
	}
	//设置控件属性
	roomListBack->loadTexture("image/roomListBack.png");

	joinButton->addTouchEventListener(CC_CALLBACK_1(Room::clickJoinCallBack, this, _chosenRoom));
	joinButton->setTitleFontSize(30);
	//	joinButton->setTitleFontName("fonts/Marker Felt.ttf");
	backButton->addTouchEventListener(CC_CALLBACK_1(Room::clickRoomListBackCallBack, this));
	backButton->setTitleFontSize(30);
	//	backButton->setTitleFontName("fonts/Marker Felt.ttf");
	refreshButton->addTouchEventListener(CC_CALLBACK_1(Room::clickRoomListRefreshCallBack, this));
	refreshButton->setTitleFontSize(30);
	//	refreshButton->setTitleFontName("fonts/Marker Felt.ttf");
	
	/*
	 *设置房间列表信息
	 *暂拟通过Room的private变量_rooms获取所有已经创建的房间信息
	 */
	int roomNum = _rooms.size();  //获取房间数量
	for (int i = 0; i < roomNum; ++i)
	{
		auto listText = dynamic_cast<ui::Text*>(roomListMenu->getChildByTag(10 + i));
		__String* title = __String::createWithFormat("%s :%d",_rooms[i].getRoomName().c_str(),_rooms[i].getPlayerNum());
		listText->setString(title->getCString());
	}

	this->addChild(_backColor);
	this->addChild(_roomListLayer, 1);
}

void Room::removeRoomList()
{
	this->removeChild(_roomListLayer);
	this->removeChild(_backColor);
}

void Room::addSetRoomNameLayer()
{
    _backColor = LayerColor::create();
	_backColor->initWithColor(Color4B(162, 162, 162, 128));
	
	_setRoomNameLayer = CSLoader::getInstance()->createNode("cocosstudio/setRoomName.csb");
	auto backImage = dynamic_cast<ui::ImageView*>(_setRoomNameLayer->getChildByName("backImage"));
	auto textBack = dynamic_cast<ui::ImageView*>(backImage->getChildByName("textBack"));
	auto inputField = dynamic_cast<ui::TextField*>(textBack->getChildByName("inputField"));
	auto okButton = dynamic_cast<ui::Button*>(backImage->getChildByName("okButton"));
	auto backButton = dynamic_cast<ui::Button*>(backImage->getChildByName("backButton"));

	textBack->loadTexture("image/textBack.png");
	inputField->addTouchEventListener(CC_CALLBACK_1(Room::clickSetRoomInputCallBack,this,inputField));

	backImage->loadTexture("image/setRoomBack.png");
	
	okButton->setTitleFontSize(30);
	okButton->addTouchEventListener(CC_CALLBACK_1(Room::clickSetRoomOkCallBack, this, inputField));
	//	okButton->setTitleFontName("fonts/Marker Felt.ttf");
	backButton->setTitleFontSize(30);
	backButton->addTouchEventListener(CC_CALLBACK_1(Room::clickSetRoomBackCallBack, this));
	//	backButton->setTitleFontName("fonts/Marker Felt.ttf");

	this->addChild(_backColor);
	this->addChild(_setRoomNameLayer, 1);
}

void Room::removeSetRoomNameLayer()
{
	this->removeChild(_setRoomNameLayer);
	this->removeChild(_backColor);
}

void Room::addReadyRoomLayer(const std::string& name)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	//读取控件
	_readyRoomLayer = CSLoader::getInstance()->createNode("cocosstudio/readyRoomLayer.csb");
	auto readyRoomBack = dynamic_cast<ui::ImageView*>(_readyRoomLayer->getChildByName("readyRoomBack"));
	auto roomState = dynamic_cast<ui::Layout*>(readyRoomBack->getChildByName("roomState"));
	auto startButton = dynamic_cast<ui::Button*>(readyRoomBack->getChildByName("startButton"));
	auto backButton = dynamic_cast<ui::Button*>(readyRoomBack->getChildByName("backButton"));
	auto nameText = dynamic_cast<ui::Text*>(roomState->getChildByName("name"));
	auto playerText = dynamic_cast<ui::Text*>(roomState->getChildByName("player"));

	//设置控件属性
	readyRoomBack->loadTexture("image/readyRoomBack.png");

	startButton->setTitleFontSize(30);
	startButton->setTitleColor(Color3B::BLACK);
	//	startButton->setTitleFontName("fonts/Marker Felt.ttf");
	startButton->addTouchEventListener(CC_CALLBACK_1(Room::clickStartCallBack, this, _currentMapTag));
	backButton->setTitleFontSize(30);
	backButton->setTitleColor(Color3B::BLACK);
	//	backButton->setTitleFontName("fonts/Marker Felt.ttf");
	backButton->addTouchEventListener(CC_CALLBACK_1(Room::clickReadyRoomBackCallBack, this));

	//添加设置地图信息
	__String* mapName = __String::createWithFormat("image/map%d.png", _currentMapTag);
	auto mapWindow = Sprite::create(mapName->getCString());
	mapWindow->setPosition(visibleSize.width*0.3, visibleSize.height*0.6);
	_readyRoomLayer->addChild(mapWindow, 1);

	/*
	 *设置该房间名称及玩家数量
	 *暂拟通过Room的private变量_rooms查找该房间信息
	 *玩家数量信息当前为静态量,与服务器的实时信息交互待添加。
	 */
	auto room = _rooms.getRoomByName(name);  //获取该房间对象
	if (room != _rooms.end())
	{
		nameText->setString(room->getRoomName());
		__String* playerNum = __String::createWithFormat("%d", room->getPlayerNum());
		playerText->setString(playerNum->getCString());
	}
	else
	{
		nameText->setString("emptyRoom");
	}
	this->addChild(_readyRoomLayer);
}

void Room::removeReadyRoomLayer()
{
	this->removeChild(_readyRoomLayer);
}

void Room::sendStop()
{
	namespace ip = boost::asio::ip;
	boost::asio::io_service io_service;

	ip::udp::socket socket(io_service, ip::udp::endpoint(g_clientEndpoint[0].address(), 6007));
	ip::udp::endpoint receive_point(g_clientEndpoint[0].address(), 6003);

	char message[5] = "stop";
	socket.send_to(boost::asio::buffer(message, strlen(message) + 1), receive_point);

	socket.close();
}

void Room::initBroadcast(Room* ptr)
{
	namespace ip = boost::asio::ip;
	boost::asio::io_service io_service;

	ip::udp::socket socket(io_service, ip::udp::endpoint(ip::udp::v4(), 0));
	socket.set_option(boost::asio::socket_base::broadcast(true));
	ip::udp::endpoint broadcast_endpoint(ip::address_v4::broadcast(), 6001);

	ptr->_broadRunning = true;

	char buf[80];
	int icount = 1;
	while (ptr->_clientNum < 4 && ptr->_broadRunning)
	{
		sprintf(buf, "room: %s, player: %d, map: %d, mapSeed: %d",ptr->_myname.c_str(), ptr->_clientNum, ptr->_currentMapTag, g_mapSeed);
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
		if (std::string(buf) == "stop")
			break;
		if (std::string(buf) != "connect")
			continue;
		ip::udp::endpoint client_point(sender_endpoint.address(), 6001);
		g_clientEndpoint.push_back(sender_endpoint);
		ptr->_clientNum++;

		char sendBuf[50];
		sprintf(sendBuf, "successfully connected! player%d", ptr->_clientNum);
		socket.send_to(boost::asio::buffer(sendBuf, strlen(sendBuf) + 1), client_point);
		sprintf(sendBuf, "%d", g_mapSeed);
		socket.send_to(boost::asio::buffer(sendBuf, strlen(sendBuf) + 1), client_point);
		//

		boost::this_thread::sleep(boost::posix_time::seconds(1));
		log("reveiver running");
	}

	socket.close();
}

void Room::initClient(Room* ptr)
{
	namespace ip = boost::asio::ip;
	boost::asio::io_service io_service;

	ip::udp::socket socket(io_service, ip::udp::endpoint(ip::udp::v4(), 6001));
	ip::udp::endpoint sender_endpoint;

	char buf[80];
	while (1)
	{
		std::size_t bytes_transferred = socket.receive_from(boost::asio::buffer(buf), sender_endpoint);
		log("got %d bytes.", bytes_transferred);
		log("the message: %s", buf);

		std::string checkBuf(buf);
		if (checkBuf.find("room") != std::string::npos)
		{
			auto posBegin = checkBuf.find(":");
			auto posEnd = checkBuf.find(",");
			auto roomName = checkBuf.substr(posBegin + 2, posEnd - posBegin - 2);

			posBegin = checkBuf.find(":", posEnd);
			auto subNum = checkBuf.substr(posBegin + 2, 1);
			auto playerNum = atoi(subNum.c_str());

			if (ptr->_rooms.getRoomByName(roomName) == ptr->_rooms.end())
			{
				ptr->_rooms.pushBack(RoomItem(roomName, playerNum));
			}
			else
			{
				ptr->_rooms.getRoomByName(roomName)->setPlayerNum(playerNum);
			}

		}
		g_serverEndpoint = sender_endpoint;

		std::string checkStr(buf);
		if (checkStr.find("success") != std::string::npos)
		{
			log("success");
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
			g_mapSeed = static_cast<int>(strtol(buf, NULL, 10));
			log("deal message: %d", g_mapSeed);
			return;
		}

		if (ptr->_isOwner || (ptr->_joinRoom&&checkBuf.find(ptr->_chosenRoom) != std::string::npos))
		{
			char *message = "connect";
			ip::udp::socket sender(io_service, ip::udp::endpoint(ip::udp::v4(), 6005));
			ip::udp::endpoint server_point(sender_endpoint.address(), 6003);
			sender.send_to(boost::asio::buffer(message, strlen(message) + 1), server_point);
			sender.close();
			log("connect");
			ptr->_joinRoom = false;
			ptr->_isOwner = false;

			auto posBegin = checkBuf.find("map");
			auto mapNum = checkBuf.substr(posBegin + 5, 1);
			g_mapName = atoi(mapNum.c_str());
		}

		boost::this_thread::sleep(boost::posix_time::seconds(1));
		log("client running");
	}

	socket.close();
}