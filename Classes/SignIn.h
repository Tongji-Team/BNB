#ifndef __SIGN_IN_H__
#define __SIGN_IN_H__

#include "cocos2d.h"
#include"cocos-ext.h"
#include"StartScene.h"

USING_NS_CC_EXT;

class SignIn : public cocos2d::Layer, public cocos2d::TextFieldDelegate
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	bool touchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void touchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
	void touchEnded(cocos2d::Touch *touch, cocos2d::Event *event);

	// 当用户启动虚拟键盘的时候回调函数
	virtual bool onTextFieldAttachWithIME(TextFieldTTF * sender);
	// 当用户关闭虚拟键盘的时候回调函数
	virtual bool onTextFieldDetachWithIME(TextFieldTTF * sender);
	// 当用户进行输入 虚拟键盘的时候回调函数
	virtual bool onTextFieldInsertText(TextFieldTTF * sender, const char * text, size_t nLen);
	// 当用户进行删除文字 虚拟键盘的时候回调函数
	virtual bool onTextFieldDeleteBackward(TextFieldTTF * sender, const char * delText, size_t nLen);

	void ClickInCall(Ref* obj);
	void ClickUpCall(Ref* obj);

	CREATE_FUNC(SignIn);

private:
	TextFieldTTF* _Name;
	TextFieldTTF* _Password;

};

#endif /