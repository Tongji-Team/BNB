#ifndef __RoomObj_H__
#define __RoomObj_H__
#include<string>
#include<vector>

class RoomItem
{
public:
	RoomItem();
	RoomItem(std::string name, int num = 0, int ip = 0) :_roomName(name), _playerNum(num), _ipAddr(ip){}

	std::string getRoomName() const;
	int getPlayerNum() const;
	int getIP() const;

	void setPlayerNum(int num);
	void setIP(int ip);

private:
	std::string _roomName; //房间名
	int _playerNum = 0;  //玩家数量
	int _ipAddr = 0;  //IP地址
};

class RoomVec
{
public:
	RoomVec(){};
	RoomVec(const RoomItem& r);

	typedef std::vector<RoomItem>::iterator iterator;

	int size() const;
	void pushBack(const RoomItem& r);
	iterator begin();
	iterator end();
	iterator getRoomByName(const std::string& s);
	iterator getRoomByIP(int i);
	RoomItem& operator[](int i);
private:
	std::vector<RoomItem> rooms;
};
#endif