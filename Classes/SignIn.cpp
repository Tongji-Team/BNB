#include"SignIn.h"
#include<fstream>

USING_NS_CC;

Scene* SignIn::createScene()
{
	auto scene = Scene::create();

	auto layer = SignIn::create();

	scene->addChild(layer);

	return scene;
}

bool SignIn::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg = Sprite::create("image/bg-st.png");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(bg, 0);

	//设置name 
	_Name = TextFieldTTF::textFieldWithPlaceHolder("Name", "Arial", 30);
	_Name->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height*0.75));
	_Name->setColorSpaceHolder(Color3B::BLACK);
	_Name->setDelegate(this);
	
	addChild(_Name);

	_Password = TextFieldTTF::textFieldWithPlaceHolder("Password", "Arial", 30);
	_Password->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height*0.5));
	_Password->setSecureTextEntry(true);
	_Password->setColorSpaceHolder(Color3B::BLACK);
	_Password->setDelegate(this);

	addChild(_Password);

	auto signInLabel = Label::createWithTTF("Sign in", "fonts/Marker Felt.ttf", 50);
	auto signInItem = MenuItemLabel::create(signInLabel, CC_CALLBACK_1(SignIn::ClickInCall, this));

	auto signUpLabel = Label::createWithTTF("Sign up", "fonts/Marker Felt.ttf", 50);
	auto signUpItem = MenuItemLabel::create(signUpLabel, CC_CALLBACK_1(SignIn::ClickUpCall, this));

	auto menu = Menu::create(signInItem,signUpItem, nullptr);
	menu->alignItemsHorizontally();
	menu->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height*0.25));
	this->addChild(menu);
	
}

//sign up操作：新的账号存入本地，后返回Startscene
//存储路径为平台默认"可写入目录"下的"\data\Users.txt"
//win32平台存储路径："\proj.win32\Debug.win32\data\Users.txt"
void SignIn::ClickUpCall(Ref* obj)
{
	log("Sign ");
	auto sharedFileUtils = FileUtils::getInstance();
	std::string writablePath = sharedFileUtils->fullPathForFilename("/data/Users.txt");
	std::ofstream outfile(writablePath.c_str(),std::ofstream::app);
	outfile << _Name->getString() << " " << _Password->getString() << std::endl;

	auto scene = StartScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

//sign in操作：返回Startscene
void SignIn::ClickInCall(Ref* obj)
{
	log("Sign in");
	auto scene = StartScene::createScene();
	Director::getInstance()->replaceScene(scene);
}


void SignIn::onEnter()
{
	Layer::onEnter();
	log("Sign onEnter");

	auto listener = EventListenerTouchOneByOne::create();

	//设置回调函数
	listener->onTouchBegan = CC_CALLBACK_2(SignIn::touchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(SignIn::touchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(SignIn::touchEnded, this);

	//注册监听器
	EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _Name);
	eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), _Password);
}

bool SignIn::touchBegan(Touch* touch, Event* event)
{
	log("onTouchBegin");
	auto target = static_cast<TextFieldTTF*>(event->getCurrentTarget());

	//获取并判断触摸点是否在文本框内
	if (target->getBoundingBox().containsPoint(touch->getLocation()))
	{
		target->attachWithIME();
	}
	else
	{
		target->detachWithIME();
	}
	return true;
}

void SignIn::touchMoved(Touch *touch, Event *event)
{
	log("onTouchMoved");
}

void SignIn::touchEnded(Touch *touch, Event *event)
{
	log("onTouchEnded");
	
}

void SignIn::onExit()
{
	Layer::onExit();
	log("SignIn onExit");
	Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(_Name);
	Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(_Password);
}


bool SignIn::onTextFieldAttachWithIME(TextFieldTTF *pSender)
{
	log("onAttach");
	pSender->setColorSpaceHolder(Color3B::WHITE);
	pSender->setColor(Color3B::WHITE);
	return false;                       
}

bool SignIn::onTextFieldDetachWithIME(TextFieldTTF *pSender)
{
	log("detach");
	pSender->setColorSpaceHolder(Color3B::BLACK);
	pSender->setColor(Color3B::BLACK);
	return false;
}

bool SignIn::onTextFieldInsertText(TextFieldTTF *pSender, const char *text, size_t nLen)
{
	log("inserting text");
	 //空格和\n作为输入结束符  
    if (*text=='\n'||*text == ' ')  
    {  
        pSender->detachWithIME(); 
        return true; 
    }  
	return false;
}

bool SignIn::onTextFieldDeleteBackward(TextFieldTTF* sender, const char* delText, size_t nLen)
{
	log("delete text");
	return false;
}