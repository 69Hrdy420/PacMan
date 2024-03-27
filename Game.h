#pragma once
#include <vector>
#include "Map_Structures.h"
#include "Entity.h"
#include "Window2.h"

class Game
{
public:
	Game(sf::RenderWindow& window, const std::vector<Line>& walls, std::vector<Entity*>& entities, const std::vector<std::vector<Peer>>& peers);
	void run();
	Object eat(sf::Vector2f pos);
private:
	std::vector<Line> walls;
	std::vector<Entity*>& entities;
	std::vector<std::vector<Peer>> peers;
	std::chrono::high_resolution_clock::time_point last_update;
	sf::RenderWindow& window;

	sf::Vector2i get_peer(sf::Vector2f pos);
	bool try_move(Entity& entity, Direction direction, float distance);
	void check_collisons();
	
	void draw();
	void draw_walls();
	void draw_objects();
	void draw_entities();
};

