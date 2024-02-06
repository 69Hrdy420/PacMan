#pragma once
#include <SFML/Graphics.hpp>
#include "Shapes.h"
#include "Window.h"

class BodyPart
{
public:
	BodyPart(sf::ConvexShape& shape, sf::Vector2f local_pos);
	BodyPart(const BodyPart& other);
	BodyPart& operator=(const BodyPart& other);
	sf::Vector2f local_pos;
	void draw(sf::Vector2f pos, sf::Color color);
private:
	sf::ConvexShape& shape;
	//const Shape_Name& index;
};

