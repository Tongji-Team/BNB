#ifndef __MainScene_H__
#define __MainScene_H__
#include "cocos2d.h"
#include "Player.h"
#include "Item.h"
#include <boost/thread/thread.hpp>

USING_NS_CC;

class MainScene :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void onEnter();

	CREATE_FUNC(MainScene);

	void setPhysicsWorld(PhysicsWorld* physicsWorld){ _world = physicsWorld; }//添加物理引擎

	Player* addRole(float x,float y);

	void addPlayer();

	void addListener();

	void update(float dt) override;

	bool addMap();

	void addItem();

	cocos2d::Point tileCoordFromPosition(cocos2d::Point position);//将像素坐标点转换为地图瓦片坐标点

	bool checkCollidable(Point, Player*);
	bool onContactBegin(const PhysicsContact& contact);

	Point getBombPosition(Point coord);

	void removeBlock(Point coord);

	void static initServer(MainScene*);
	void static initClientSend(MainScene*);
	void static initClientReceive(MainScene*);
	void static dealMessage(char*, MainScene*);

	void placeBomb(Player*);

	cocos2d::CCTMXTiledMap *_tileMap;
	cocos2d::TMXLayer* _item;//保存道具层对象

	std::vector<std::vector<Vec2>> _mapCoord;
	std::vector<std::vector<int>> _mapProp;

	Vector<Player*> _playerGroup;

private:
	PhysicsWorld* _world;

	Player* _player;

	std::string _playerID;

	EventListenerKeyboard* _listener_key;
	EventListenerPhysicsContact* _listener_contact;

	boost::thread_group _threadGroup;
};

#endif