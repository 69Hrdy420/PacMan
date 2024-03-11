#pragma once
#include "BodyPart.h"
#include <chrono>

class Entity
{
public:
	Entity(sf::Color color, std::chrono::high_resolution_clock::time_point game_start);
	virtual void draw(sf::RenderWindow& window);
	virtual void animate(std::chrono::milliseconds now) {}
	sf::Vector2f pos;
	sf::Color color;
protected:
	std::vector<BodyPart> body;
	std::chrono::high_resolution_clock::time_point last_animation;
};

