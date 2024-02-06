#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>

class Peer
{
public:
	//Peer(bool route, bool coin);
	Peer(uint8_t x_pos, uint8_t y_pos, bool route, bool coin);
	static std::vector<std::vector<Peer>> map;
	static std::vector<std::vector<bool>> coins;
	const bool route;
private:
	//uint8_t x, y;
};

