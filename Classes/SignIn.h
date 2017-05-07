#ifndef __SIGN_IN_H__
#define __SIGN_IN_H__

#include "cocos2d.h"
#include"StartScene.h"


class SignIn : public cocos2d::Layer, public cocos2d::TextFieldDelegate
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();  
	//��ӶԵ���������¼��ļ���
	virtual void onEnter();
	//�������
	virtual void onExit();
	
	//����Ե����û��������ı���ļ���
	bool touchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void touchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
	void touchEnded(cocos2d::Touch *touch, cocos2d::Event *event);

	//�����û�������������̵Ļص�����
	virtual bool onTextFieldAttachWithIME(TextFieldTTF * sender);
	virtual bool onTextFieldDetachWithIME(TextFieldTTF * sender);
	virtual bool onTextFieldInsertText(TextFieldTTF * sender, const char * text, size_t nLen);
	virtual bool onTextFieldDeleteBackward(TextFieldTTF * sender, const char * delText, size_t nLen);

	//signin��signup�ĵ���ص����� 
	void clickInCall(Ref* obj);
	void clickUpCall(Ref* obj);

	CREATE_FUNC(SignIn);
	//����ָ���ϼ�startscene��ָ��
	void setStartSce(StartScene*);

private:
	TextFieldTTF* _Name;  //�û���
	TextFieldTTF* _Password;  //����
	StartScene* _startSce = nullptr;  //ָ���ϼ�startscene��ָ��
};

#endif 