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

	// ���û�����������̵�ʱ��ص�����
	virtual bool onTextFieldAttachWithIME(TextFieldTTF * sender);
	// ���û��ر�������̵�ʱ��ص�����
	virtual bool onTextFieldDetachWithIME(TextFieldTTF * sender);
	// ���û��������� ������̵�ʱ��ص�����
	virtual bool onTextFieldInsertText(TextFieldTTF * sender, const char * text, size_t nLen);
	// ���û�����ɾ������ ������̵�ʱ��ص�����
	virtual bool onTextFieldDeleteBackward(TextFieldTTF * sender, const char * delText, size_t nLen);

	void ClickInCall(Ref* obj);
	void ClickUpCall(Ref* obj);

	CREATE_FUNC(SignIn);

private:
	TextFieldTTF* _Name;
	TextFieldTTF* _Password;

};

#endif /