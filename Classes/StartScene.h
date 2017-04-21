#ifndef __StartScene_H__
#define __StartScene_H__
#include "cocos2d.h"

USING_NS_CC;

class StartScene : public Layer
{
public:
	bool init();
	static Scene* createScene();
	void onStart(Ref* obj);
	void onSign(Ref* obj);
	CREATE_FUNC(StartScene);
};

#endif