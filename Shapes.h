#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Options.h"

enum Shape_Name
{
	GHOST_BODY = 0, GHOST_LEGS1 = 1, GHOST_LEGS2 = 2, PACMAN = 3
};

class Shapes
{
public:
	static sf::ConvexShape& get(int name);
private:
	static std::vector<sf::ConvexShape> shapes;
	Shapes();
	void create_ghost_upperbody();
	void create_ghost_legs();
	void create_pacman();
	static Shapes instance;
};

