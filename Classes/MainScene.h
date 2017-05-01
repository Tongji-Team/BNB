#ifndef __MainScene_H__
#define __MainScene_H__
#include "cocos2d.h"
#include "Player.h"
#include <vector>

USING_NS_CC;

class MainScene :public cocos2d::Layer
{
	cocos2d::CCTMXTiledMap *_tileMap;
	cocos2d::TMXLayer* _collidable;//保存碰撞层对象

public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void onEnter();

	CREATE_FUNC(MainScene);

	void setPhysicsWorld(PhysicsWorld* physicsWorld){ _world = physicsWorld; }//添加物理引擎

	void addRole(float x,float y);

	void addListener();

	void update(float dt) override;

	bool addMap();

	cocos2d::Point tileCoordFromPosition(cocos2d::Point position);//将像素坐标点转换为地图瓦片坐标点

	bool checkCollidable(Point pos);

	Point getBombPosition(Point coord);
	std::vector<std::vector<Vec2>> _mapCoord;

private:
	PhysicsWorld* _world;

	Player* _player;

	EventListenerKeyboard* _listener_key;
};

#endif