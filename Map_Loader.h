#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <unordered_set>
#include "Map_Structures.h"
#include "Options.h"



class Map_Loader
{
public:
	Map_Loader(const std::unordered_set<Line, LineHash>& paths, const std::vector<std::vector<Object>>& objects, const std::vector<int>& horizontal_bridges, const std::vector<int>& vertical_bridges);
	void run();
private:
	void draw();
	void draw_walls();
	void draw_objects();

	std::unordered_set<Line, LineHash> load_walls(const std::unordered_set<Line, LineHash>& paths);
	void load_horizontal_wall(const Line& line, std::unordered_set<Line, LineHash>& set, const std::unordered_set<Line, LineHash>& paths);
	void load_vertical_wall(const Line& line, std::unordered_set<Line, LineHash>& set, const std::unordered_set<Line, LineHash>& paths);
	std::vector<Line> load_optimized_walls(std::unordered_set<Line, LineHash>&& walls);
	bool is_horizontal(const Line& line);
	bool is_vertical(const Line& line);
	std::unordered_set<Line, LineHash> load_bridges(std::unordered_set<Line, LineHash>&& walls, const std::vector<int>& horizontal_bridges, const std::vector<int>& vertical_bridges);
	std::vector<std::vector<Peer>> load_peers(const std::unordered_set<Line, LineHash>& paths, sf::Vector2i size);
	std::vector<Line> walls;
	std::vector<std::vector<Peer>> peers;
	sf::RenderWindow window;
};