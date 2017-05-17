#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "cocos2d.h"
#include "Bomb.h"

USING_NS_CC;

class Player :public Sprite
{
public:
	static Player* create();

	bool init();

	//接下来四个函数用于获取人物移动方向的信息
	bool getLeft(){ return _left; }
	bool getRight(){ return _right; }
	bool getUp(){ return _up; }
	bool getDown(){ return _down; }

	//接下来四个函数用于改变人物移动方向的信息
	void alterLeft(){ _left = (_left == false ? true : false); }
	void alterRight(){ _right = (_right == false ? true : false); }
	void alterUp(){ _up = (_up == false ? true : false); }
	void alterDown(){ _down = (_down == false ? true : false); }

	//接下来四个函数用于使人物向四个方向移动
	Point walkLeft();
	Point walkRight();
	Point walkUp();
	Point walkDown();

	//接下来的函数用于处理炸弹
	int getBombPresentNum(){ return _bombPresentNum; }
	int getBombMaxNum(){ return _bombMaxNum; }
	int getBombPower(){ return _bombPower; }
	void increaseBombNum(){ ++_bombPresentNum; }
	void decreaseBombNum(){ --_bombPresentNum; }
	Bomb* getBomb(){ return _bomb; }

	Bomb* addBomb(int power, Vec2 pos);
	bool eatItem(int type);

	//接下来的四个bool型变量用于描述人物的移动方向
	bool _left;
	bool _right;
	bool _up;
	bool _down;

private:
	bool _canMove;//这个属性用于描述人物是否可以移动
	int _speed;//用于描述人物的移动速度
	int _bombMaxNum;//用于描述放置炸弹的数量上限
	int _bombPresentNum;//用于计算现在已经放置了几个炸弹
	int _bombPower;//用于衡量炸弹的威力；

	Bomb* _bomb;
};

#endif