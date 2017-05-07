#ifndef __SIGN_IN_H__
#define __SIGN_IN_H__

#include "cocos2d.h"
#include"StartScene.h"


class SignIn : public cocos2d::Layer, public cocos2d::TextFieldDelegate
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();  
	//添加对单击输入框事件的监听
	virtual void onEnter();
	//清除监听
	virtual void onExit();
	
	//处理对单击用户名密码文本框的监听
	bool touchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void touchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
	void touchEnded(cocos2d::Touch *touch, cocos2d::Event *event);

	//控制用户名密码输入过程的回调函数
	virtual bool onTextFieldAttachWithIME(TextFieldTTF * sender);
	virtual bool onTextFieldDetachWithIME(TextFieldTTF * sender);
	virtual bool onTextFieldInsertText(TextFieldTTF * sender, const char * text, size_t nLen);
	virtual bool onTextFieldDeleteBackward(TextFieldTTF * sender, const char * delText, size_t nLen);

	//signin，signup的点击回调函数 
	void clickInCall(Ref* obj);
	void clickUpCall(Ref* obj);

	CREATE_FUNC(SignIn);
	//设置指向上级startscene的指针
	void setStartSce(StartScene*);

private:
	TextFieldTTF* _Name;  //用户名
	TextFieldTTF* _Password;  //密码
	StartScene* _startSce = nullptr;  //指向上级startscene的指针
};

#endif 