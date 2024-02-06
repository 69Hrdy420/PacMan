#include <string>

#include "Peer.h"


class Map
{
public:
	Map();
	bool load(uint16_t num);
	bool create(std::string name);
	bool erase(uint16_t num);
	const std::vector<std::string>& names;
private:
	std::vector<std::string> Names;
};