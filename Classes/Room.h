#ifndef __ROOM_H__
#define __ROOM_H__

#include "cocos2d.h"
#include"RoomObj.h"
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include<string>
#include "ui/cocosGUI.h"

class Room : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void clickInitMenuCallBack(Ref* obj);
	void clickSelectedMenuCallBack(Ref* obj,int mapNum);
	void clickCreatRoomCallBack(Ref* obj,int mapNum);
	void clickFindRoomCallBack(Ref* obj,int mapNum);
	void clickStartCallBack(Ref* obj, int mapNum);
	void clickRoomListTextCallBack(Ref* obj, cocos2d::ui::ListView* list,int tag);
	void clickRoomBackCallBack(Ref* obj);
	void clickRoomListBackCallBack(Ref* obj);
	void clickSetRoomOkCallBack(Ref* obj,cocos2d::ui::TextField* inputField);
	void clickSetRoomInputCallBack(Ref* obj, cocos2d::ui::TextField* inputField);
	void clickSetRoomBackCallBack(Ref* obj);
	void clickJoinCallBack(Ref* obj,std::string& roomName);
	void clickReadyRoomBackCallBack(Ref* obj);

	void addSelectedMenu();
	void removeSelectedMenu();
	void addMapNames();
	void changeMapWindow();
	void addRomeList();
	void removeRoomList();
	void addSetRoomNameLayer();
	void removeSetRoomNameLayer();
	void addReadyRoomLayer(const std::string& name);
	void removeReadyRoomLayer();

	void static initBroadcast(Room* ptr);
	void static initClient(Room* ptr);
	void static initReceiver(Room* ptr);

	void makeMapSeed();

	boost::thread_group _threadGroup;

	CREATE_FUNC(Room);
private:
	//地图选择控件
	cocos2d::Menu* _initMenu; 
	cocos2d::Menu* _selectedMenu;
	cocos2d::Label* _selectedMap;
	std::vector<std::string> _mapNames;
	cocos2d::Sprite* _mapWindow;
	int _currentMapTag = 0;
	bool mapMenuIsShow = 0;
	//网络通信控件
	int _clientNum = 0;
	//房间名设置控件
	cocos2d::Node* _setRoomNameLayer;
	cocos2d::LayerColor* _backColor;
	//房间列表控件
	cocos2d::Node* _roomListLayer;
	//准备房间控件
	cocos2d::Node* _readyRoomLayer;
	//所有以创建房间
	RoomVec _rooms;//= RoomItem("try",1);
	//find room 中被选中的房间
	std::string _chosenRoom;
};

#endif 