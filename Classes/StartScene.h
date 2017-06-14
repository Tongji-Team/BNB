#ifndef __StartScene_H__
#define __StartScene_H__
#include "cocos2d.h"

USING_NS_CC;

class StartScene : public Layer
{
public:
	bool init();
	static Scene* createScene();

	void addMenu();
	void addSignLabel();
	void addHelpLayer();
	void removeHelpLayer();

	void clickStartCallBack(Ref* obj);
	void clickSignCallBack(Ref* obj);
	void clickHelpCallBack(Ref* obj);
	void clickHelpBackCallBack(Ref* obj);

	CREATE_FUNC(StartScene);
};

#endif