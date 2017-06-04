#include "StartScene.h"
#include "VisibleRect.h"
#include "MainScene.h"
#include"SignIn.h"
#include"Room.h"
#include "cocostudio/CocoStudio.h"
#include "ui/cocosGUI.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
using namespace cocostudio;
bool StartScene::init()
{
	if (!Layer::init())
		return false;

	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto node = CSLoader::getInstance()->createNode("cocosstudio/startScene.csb");
	//初始化背景图片
	auto background = dynamic_cast<ui::ImageView*>(node->getChildByName("background"));
	background->loadTexture("image/startBack.png");
	//读取菜单各个控件
	auto menuLayout = dynamic_cast<ui::Layout*>(node->getChildByName("startMenu"));
	std::vector<ui::ImageView*> menusImages;
	menusImages.push_back(dynamic_cast<ui::ImageView*>(menuLayout->getChildByName("Image_1")));
	menusImages.push_back(dynamic_cast<ui::ImageView*>(menuLayout->getChildByName("Image_2")));
	menusImages.push_back(dynamic_cast<ui::ImageView*>(menuLayout->getChildByName("Image_3")));
	for (int i = 0; i < 3; ++i){
		menusImages[i]->loadTexture("image/menuPart.png");
	}
	std::vector<ui::Text*> menus;
	menus.push_back(dynamic_cast<ui::Text*>(menusImages[0]->getChildByName("Start")));
	menus.push_back(dynamic_cast<ui::Text*>(menusImages[1]->getChildByName("Sign")));
	menus.push_back(dynamic_cast<ui::Text*>(menusImages[2]->getChildByName("Room")));
	//初始化菜单事件
	menus[0]->addTouchEventListener(CC_CALLBACK_1(StartScene::onStart, this));
	menus[1]->addTouchEventListener(CC_CALLBACK_1(StartScene::onSign, this));
	menus[2]->addTouchEventListener(CC_CALLBACK_1(StartScene::onRoom, this));
	//初始化自定义字体
	for (int i = 0; i < 3; ++i)
	{
		menus[i]->setFontName("fonts/Marker Felt.ttf");
	}
	this->addChild(node);

	//初始化登录回显标签
	auto signLabel = LabelTTF::create("Hello tourist! Please sign in.", "Arial", 20);
	signLabel->setColor(Color3B::BLUE);
	signLabel->setAnchorPoint(Vec2(0, 0));
	Size signSize = signLabel->getDimensions();
	signLabel->setPosition(Vec2(visibleSize.width - signSize.width - 300, visibleSize.height - signSize.height - 40));
	this->addChild(signLabel, 1, 100);

	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/music.wav", true);
	
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
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/readygo.wav", false);
}

void StartScene::onSign(Ref* obj)
{
	log("Sign");
	auto scene = Scene::create();
	auto layer = SignIn::create();
	layer->setStartSce(this);
	scene->addChild(layer);
	auto reScene = TransitionShrinkGrow::create(0.6f, scene);
	Director::getInstance()->pushScene(reScene);
}

void StartScene::onRoom(Ref* obj)
{
	log("Room");
	auto scene = Room::createScene();
	auto reScene = TransitionShrinkGrow::create(0.6f, scene);
	Director::getInstance()->replaceScene(reScene);
}
