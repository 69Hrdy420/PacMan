#pragma once
#include "Map_Loader.h"

class Map_Editor
{
public:
	Map_Editor(sf::RenderWindow& window, uint16_t width, uint16_t height);
	void run();
private:
	struct Path_Buffer
	{
		bool is_active = false, is_horizontal, is_creating;
		int length = 0;
		sf::Vector2i pos;
	}; 
	void resize_map(uint16_t x, uint16_t y);
	void key_pressed(sf::Keyboard::Key key);
	void print();
	void load(const std::vector<int>& data);
	std::vector<int> save();

	sf::RenderWindow& window;
	sf::RectangleShape local_bounds;
	sf::Vector2i mouse_pos;

	std::vector<std::vector<Object>> objects;
	Object object;
	bool place_object = false;

	std::unordered_set<Line, LineHash> lines;
	Path_Buffer path_buffer;
	void update_buffer(sf::Vector2i mouse_pos);
	std::vector<Line> get_lines_from_buffer();
	std::vector<bool> horizontal_bridges;
	std::vector<bool> vertical_bridges;
	std::vector<int> load_bridges(const std::vector<bool>& bridges);

	void draw(); 
	void draw_paths();
	void draw_objects();
	void draw_bridges();

	sf::Vector2i get_peer(sf::Vector2i mouse_pos);
	sf::Vector2i get_peer_pos(sf::Vector2i peer);
};