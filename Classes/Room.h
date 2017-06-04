#ifndef __ROOM_H__
#define __ROOM_H__

#include "cocos2d.h"
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
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
	void clickBackCallBack(Ref* obj);

	void addSelectedMenu();
	void removeSelectedMenu();
	void addMapNames();
	void changeMapWindow();
	void addRomeList();

	void static initBroadcast(Room* ptr);
	void static initClient(Room* ptr);
	void static initReceiver(Room* ptr);

	void makeMapSeed();

	boost::thread_group _threadGroup;

	CREATE_FUNC(Room);
private:
	cocos2d::Menu* _initMenu;
	cocos2d::Menu* _selectedMenu;
	cocos2d::Label* _selectedMap;
	std::vector<std::string> _mapNames;
	cocos2d::Sprite* _mapWindow;
	int _currentMapTag = 0;
	int _clientNum = 0;
	bool mapMenuIsShow = 0;
};

#endif 