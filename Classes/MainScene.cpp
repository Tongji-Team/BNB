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
	log("hello mainscene");

	addRole();
	addListener();

	return true;
}

void MainScene::onEnter()
{
	Layer::onEnter();
	_world->setGravity(Vec2(0, 0));
}

void MainScene::addRole()
{
	_player = Player::create();
	_player->setPosition(VisibleRect::rightBottom());
	this->addChild(_player);
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

void MainScene::update(float dt)
{
	if (_player->getLeft())
		_player->walkLeft();
	if (_player->getRight())
		_player->walkRight();
	if (_player->getUp())
		_player->walkUp();
	if (_player->getDown())
		_player->walkDown();
}