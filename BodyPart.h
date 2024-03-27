#pragma once
#include <SFML/Graphics.hpp>
#include "Shapes.h"

enum Direction
{
	UP, DOWN, LEFT, RIGHT, IDLE
};

class BodyPart
{
public:
	BodyPart(sf::ConvexShape& shape, sf::Vector2f local_pos);
	BodyPart(const BodyPart& other);
	BodyPart& operator=(const BodyPart& other);
	sf::Vector2f local_pos;
	void draw(sf::RenderWindow& window, sf::Vector2f pos, sf::Color color, int rotation);
	void resize(float scale);
private:
	sf::ConvexShape& shape;
};

