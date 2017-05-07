#include "StartScene.h"
#include "VisibleRect.h"
#include "MainScene.h"
#include"SignIn.h"
#include"Room.h"

bool StartScene::init()
{
	if (!Layer::init())
		return false;

	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto background = Sprite::create("image/bg-st.png");
	background->setPosition(VisibleRect::center());
	this->addChild(background);

	auto label1 = Label::createWithTTF("Start", "fonts/Marker Felt.ttf", 50);
	auto startItem1 = MenuItemLabel::create(label1, CC_CALLBACK_1(StartScene::onStart, this));

	auto label2 = Label::createWithTTF("Sign", "fonts/Marker Felt.ttf", 50);
	auto startItem2 = MenuItemLabel::create(label2, CC_CALLBACK_1(StartScene::onSign, this));

	auto label3 = Label::createWithTTF("Room", "fonts/Marker Felt.ttf", 50);
	auto startItem3 = MenuItemLabel::create(label3, CC_CALLBACK_1(StartScene::onRoom, this));

	auto signLabel = LabelTTF::create("Hello tourist! Please sign in.", "Arial", 20);
	signLabel->setColor(Color3B::RED);
	signLabel->setAnchorPoint(Vec2(0, 0));
	Size signSize = signLabel->getDimensions();
	signLabel->setPosition(Vec2(visibleSize.width - signSize.width - 300 ,visibleSize.height - signSize.height - 40));
	this->addChild(signLabel,1,100);

	auto menu = Menu::create(startItem1, startItem2, startItem3, nullptr);
	menu->alignItemsVertically();

	menu->setPosition(VisibleRect::center());
	this->addChild(menu);

	return true;
}

Scene* StartScene::createScene()
{
	auto scene = Scene::create();
	auto layer = StartScene::create();
	scene->addChild(layer);
	return scene;
}

void StartScene::onStart(Ref* obj)
{
	log("hello start");
	auto scene = MainScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void StartScene::onSign(Ref* obj)
{
	log("Sign");
	auto scene = Scene::create();
	auto layer = SignIn::create();
	layer->setStartSce(this);
	scene->addChild(layer);
	Director::getInstance()->pushScene(scene);
}

void StartScene::onRoom(Ref* obj)
{
	log("Room");
	auto scene = Room::createScene();
	Director::getInstance()->replaceScene(scene);
}
