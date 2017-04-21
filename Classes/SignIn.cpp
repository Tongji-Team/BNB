#include"SignIn.h"
#include<fstream>

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
	//����name����
	_editName = EditBox::create(Size(300, 100), Scale9Sprite::create("image/bg-sign.png"));
	_editName->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height*0.75));
	_editName->setFont("Arial",30);
	_editName->setFontColor(Color3B::BLACK);
	_editName->setPlaceholderFont("Arial", 30);
	_editName->setPlaceHolder("Name:");
	//name��ʽΪͬ��ѧ�ţ�7λ����.eg:1552970
	_editName->setMaxLength(7);  
	_editName->setInputMode(EditBox::InputMode::NUMERIC);
	_editName->setDelegate(this);
	addChild(_editName);

	//����password����
	_editPassword = EditBox::create(Size(300, 100), Scale9Sprite::create("image/bg-sign.png"));
	_editPassword->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height*0.5));
	_editPassword->setFont("Arial", 30);
	_editPassword->setFontColor(Color3B::BLACK);
	_editPassword->setPlaceholderFont("Arial", 30);
	_editPassword->setPlaceHolder("Password:");
	//password��ʽΪУ԰�����룬��λ����
	_editPassword->setMaxLength(6);
	_editPassword->setInputMode(EditBox::InputMode::NUMERIC);
	_editPassword->setInputFlag(EditBox::InputFlag::PASSWORD);	//����״̬����
	_editPassword->setDelegate(this);
	addChild(_editPassword);

	auto signInLabel = Label::createWithTTF("Sign in", "fonts/Marker Felt.ttf", 50);
	auto signInItem = MenuItemLabel::create(signInLabel, CC_CALLBACK_1(SignIn::ClickInCall, this));

	auto signUpLabel = Label::createWithTTF("Sign up", "fonts/Marker Felt.ttf", 50);
	auto signUpItem = MenuItemLabel::create(signUpLabel, CC_CALLBACK_1(SignIn::ClickUpCall, this));

	auto menu = Menu::create(signInItem,signUpItem, nullptr);
	menu->alignItemsHorizontally();
	menu->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height*0.25));
	this->addChild(menu);
	
}
void SignIn::editBoxEditingDidBegin(cocos2d::extension::EditBox* editBox)
{
	log("editBox %p DidBegin !", editBox);
}

void SignIn::editBoxEditingDidEnd(cocos2d::extension::EditBox* editBox)
{
	log("editBox %p DidEnd !", editBox);
}

void SignIn::editBoxTextChanged(cocos2d::extension::EditBox* editBox, const std::string& text)
{
	log("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}

void SignIn::editBoxReturn(EditBox* editBox)
{
	log("editBox %p was returned !", editBox);
}

//sign up�������µ��˺Ŵ��뱾�أ��󷵻�Startscene
//�洢·��Ϊƽ̨Ĭ��"��д��Ŀ¼"�µ�"\data\Users.txt"
//win32ƽ̨�洢·����"\proj.win32\Debug.win32\data\Users.txt"
void SignIn::ClickUpCall(Ref* obj)
{
	log("Sign ");
	auto sharedFileUtils = FileUtils::getInstance();
	std::string writablePath = sharedFileUtils->fullPathForFilename("/data/Users.txt");
	std::ofstream outfile(writablePath.c_str(),std::ofstream::app);
	outfile << _editName->getText() << " " << _editPassword->getText() << std::endl;

	auto scene = StartScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

//sign in����������Startscene
void SignIn::ClickInCall(Ref* obj)
{
	log("Sign in");
	auto scene = StartScene::createScene();
	Director::getInstance()->replaceScene(scene);
}