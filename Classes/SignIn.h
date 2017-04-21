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

	//开始编辑时  
	virtual void editBoxEditingDidBegin(EditBox* editBox);
	//结束编辑时  
	virtual void editBoxEditingDidEnd(EditBox* editBox);
	//编辑框文字改变时  
	virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
	//触发返回后  
	virtual void editBoxReturn(EditBox* editBox);

	void ClickInCall(Ref* obj);
	void ClickUpCall(Ref* obj);

	CREATE_FUNC(SignIn);

private:
	EditBox* _editName;
	EditBox* _editPassword;

};

#endif /