#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_set>
//#include "Map.h"
#include "Options.h"
#include "Hash.h"


enum Object
{
	NONE, COIN, POWERUP
};
enum Direction
{
	LEFT, RIGHT, UP, DOWN
};

struct Path_Buffer
{
	Path_Buffer()
		:pos({ -1, -1 }) {}
	Path_Buffer(sf::Vector2i pos)
		:pos(pos), length(0) {}
	Path_Buffer(Direction direction, sf::Vector2i pos)
		:pos(pos), length(0), direction(direction) {}
	Direction direction;
	uint16_t length;
	sf::Vector2i pos;
};

class Map_Editor
{
public:
	Map_Editor(sf::RenderWindow& window, uint16_t x, uint16_t y);
	void run();
private:
	void resize_map(uint16_t x, uint16_t y);
	void draw();

	Line get_line_from_buffer(sf::Color color);
	std::vector<Line> get_lines_from_buffer(sf::Color color);

	sf::Vector2i get_tile_pos(sf::Vector2f mouse_pos);
	
	sf::RenderWindow& window;
	sf::RectangleShape local_bounds;
	sf::Vector2i mouse_pos;

	std::unordered_set<Line, LineHash> lines;
	std::vector<std::vector<Object>> objects;

	Path_Buffer path_buffer;
	void update_buffer();

	bool place_object = false;
	Object object = NONE;
};
