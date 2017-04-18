#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "cocos2d.h"

USING_NS_CC;

class Player :public Sprite
{
public:
	static Player* create();

	bool init();

	//�������ĸ��������ڻ�ȡ�����ƶ��������Ϣ
	bool getLeft(){ return _left; }
	bool getRight(){ return _right; }
	bool getUp(){ return _up; }
	bool getDown(){ return _down; }

	//�������ĸ��������ڸı������ƶ��������Ϣ
	void alterLeft(){ _left = (_left == false ? true : false); }
	void alterRight(){ _right = (_right == false ? true : false); }
	void alterUp(){ _up = (_up == false ? true : false); }
	void alterDown(){ _down = (_down == false ? true : false); }

	//�������ĸ���������ʹ�������ĸ������ƶ�
	void walkLeft();
	void walkRight();
	void walkUp();
	void walkDown();

private:
	bool _canMove;//��������������������Ƿ�����ƶ�
	int _speed;//��������������ƶ��ٶ�
	int _bombMaxNum;//������������ը������������
	int _bombPresentNum;//���ڼ��������Ѿ������˼���ը��
	int _bombPower;//���ں���ը����������

	//���������ĸ�bool�ͱ�����������������ƶ�����
	bool _left;
	bool _right;
	bool _up;
	bool _down;
};

#endif