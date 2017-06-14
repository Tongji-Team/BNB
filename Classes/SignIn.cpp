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

	auto bg = Sprite::create("image/signBack.png");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(bg, 0);

	//����name 
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
	auto signInItem = MenuItemLabel::create(signInLabel, CC_CALLBACK_1(SignIn::clickInCall, this));

	auto signUpLabel = Label::createWithTTF("Sign up", "fonts/Marker Felt.ttf", 50);
	auto signUpItem = MenuItemLabel::create(signUpLabel, CC_CALLBACK_1(SignIn::clickUpCall, this));

	auto menu = Menu::create(signInItem, signUpItem, nullptr);
	menu->alignItemsHorizontally();
	menu->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height*0.25));
	this->addChild(menu);

}

//sign up�������µ��˺Ŵ��뱾�أ��󷵻�Startscene
//�洢·��Ϊƽ̨Ĭ��"��д��Ŀ¼"�µ�"\data\Users.txt"
//win32ƽ̨�洢·����"\proj.win32\Debug.win32\data\Users.txt"
void SignIn::clickUpCall(Ref* obj)
{
	log("Sign up");
//	auto sharedFileUtils = FileUtils::getInstance();
//	std::string writablePath = sharedFileUtils->fullPathForFilename("/data/Users.txt");
//	std::ofstream outfile(writablePath.c_str(), std::ofstream::app);
//	outfile << _Name->getString() << " " << _Password->getString() << std::endl;

//	LabelTTF* label = static_cast<LabelTTF*>(_startSce->getChildByTag(100));
//	std::string name = _Name->getString();
//	label->setString("Hello " + name + " !");
	Director::getInstance()->popScene();
}

//sign in����������Startscene
void SignIn::clickInCall(Ref* obj)
{
	log("Sign in");
//	LabelTTF* label = static_cast<LabelTTF*>(_startSce->getChildByTag(100));
//	std::string name = _Name->getString();
//	label->setString("Hello " + name + " !");
	Director::getInstance()->popScene();
}


void SignIn::onEnter()
{
	Layer::onEnter();
	log("enter SignIn layer");

	auto listener = EventListenerTouchOneByOne::create();

	//���ûص�����
	listener->onTouchBegan = CC_CALLBACK_2(SignIn::touchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(SignIn::touchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(SignIn::touchEnded, this);

	//ע�������
	EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _Name);
	eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), _Password);
}

bool SignIn::touchBegan(Touch* touch, Event* event)
{
	log("on touch begin");
	auto target = static_cast<TextFieldTTF*>(event->getCurrentTarget());

	//��ȡ���жϴ������Ƿ����ı�����
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
	log("on touch moved");
}

void SignIn::touchEnded(Touch *touch, Event *event)
{
	log("on touch ended");

}

void SignIn::onExit()
{
	Layer::onExit();
	log("exit SignIn scene");
	Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(_Name);
	Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(_Password);
}


bool SignIn::onTextFieldAttachWithIME(TextFieldTTF *pSender)
{
	log("attach the textfield");
	pSender->setColorSpaceHolder(Color3B::WHITE);
	pSender->setColor(Color3B::WHITE);
	return false;
}

bool SignIn::onTextFieldDetachWithIME(TextFieldTTF *pSender)
{
	log("detach the textfield");
	pSender->setColorSpaceHolder(Color3B::BLACK);
	pSender->setColor(Color3B::BLACK);
	return false;
}

bool SignIn::onTextFieldInsertText(TextFieldTTF *pSender, const char *text, size_t nLen)
{
	log("inserting text");
	//�ո��\n��Ϊ���������  
	if (*text == '\n' || *text == ' ')
	{
		pSender->detachWithIME();
		return true;
	}
	return false;
}

bool SignIn::onTextFieldDeleteBackward(TextFieldTTF* sender, const char* delText, size_t nLen)
{
	log("deleting text");
	return false;
}

void SignIn::setStartSce(StartScene* sptr)
{
	_startSce = sptr;
}