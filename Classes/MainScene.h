#ifndef __MainScene_H__
#define __MainScene_H__
#include "cocos2d.h"
#include "Player.h"

USING_NS_CC;

class MainScene :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void onEnter();

	CREATE_FUNC(MainScene);

	void setPhysicsWorld(PhysicsWorld* physicsWorld){ _world = physicsWorld; }//添加物理引擎

	void addRole();

	void addListener();

	void update(float dt) override;

private:
	PhysicsWorld* _world;

	Player* _player;

	EventListenerKeyboard* _listener_key;
};

#endif