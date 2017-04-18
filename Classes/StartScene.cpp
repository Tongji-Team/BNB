#include "StartScene.h"
#include "VisibleRect.h"

bool StartScene::init()
{
	if (!Layer::init())
		return false;
	auto background = Sprite::create("image/bg-st.png");
	background->setPosition(VisibleRect::center());
	this->addChild(background);

	auto label1 = Label::createWithTTF("Start", "fonts/Marker Felt.ttf", 50);
	auto startItem1 = MenuItemLabel::create(label1, CC_CALLBACK_1(StartScene::onStart, this));
	auto menu = Menu::create(startItem1, nullptr);
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
}