#ifndef __SIGN_IN_H__
#define __SIGN_IN_H__

#include "cocos2d.h"
#include"cocos-ext.h"
#include"StartScene.h"

USING_NS_CC_EXT;
USING_NS_CC;

class SignIn : public cocos2d::Layer, public EditBoxDelegate
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	//��ʼ�༭ʱ  
	virtual void editBoxEditingDidBegin(EditBox* editBox);
	//�����༭ʱ  
	virtual void editBoxEditingDidEnd(EditBox* editBox);
	//�༭�����ָı�ʱ  
	virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
	//�������غ�  
	virtual void editBoxReturn(EditBox* editBox);

	void ClickInCall(Ref* obj);
	void ClickUpCall(Ref* obj);

	CREATE_FUNC(SignIn);

private:
	EditBox* _editName;
	EditBox* _editPassword;

};

#endif /