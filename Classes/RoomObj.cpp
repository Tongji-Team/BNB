#include"RoomObj.h"

std::string RoomItem::getRoomName() const
{
	return _roomName;
}
int RoomItem::getPlayerNum() const
{
	return _playerNum;
}
int RoomItem::getIP() const
{
	return _ipAddr;
}

void RoomItem::setPlayerNum(int num)
{
	_playerNum = num;
}

void RoomItem::setIP(int ip)
{
	_ipAddr = ip;
}

RoomVec::RoomVec(const RoomItem& r)
{
	rooms.push_back(r);
}

void RoomVec::pushBack(const RoomItem& r)
{
	rooms.push_back(r);
}

RoomVec::iterator RoomVec::getRoomByName(const std::string& s)
{
	auto i = rooms.begin();
	while (i != rooms.end() && (i->getRoomName() != s))
		++i;
	return i;
}

RoomVec::iterator RoomVec::getRoomByIP(int ip)
{
	auto i = rooms.begin();
	while (i != rooms.end() && (i->getIP() != ip))
		++i;
	return i;
}

RoomVec::iterator RoomVec::begin()
{
	return rooms.begin();
}

RoomVec::iterator RoomVec::end()
{
	return rooms.end();
}


int RoomVec::size() const
{
	return rooms.size();
}

RoomItem& RoomVec::operator[](int i)
{
	return rooms[i];
}