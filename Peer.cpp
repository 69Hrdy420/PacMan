#include "Peer.h"

std::vector<std::vector<Peer>> Peer::map;
std::vector<std::vector<bool>> Peer::coins;

Peer::Peer(uint8_t x_pos, uint8_t y_pos, bool route, bool coin)
	:route(route)/*, x(x_pos), y(y_pos)*/
{
	//coins[x_pos][y_pos] = coin;
}
