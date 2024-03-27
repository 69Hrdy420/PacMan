#pragma once
#include "BodyPart.h"
#include <chrono>
#include <iostream>



class Entity
{
public:
	Entity(sf::Color color, const sf::Vector2f start_pos, std::chrono::high_resolution_clock::time_point game_start);
	virtual void draw(sf::RenderWindow& window);
	virtual void animate(std::chrono::milliseconds now) {}
	virtual void colision(Entity& other) {}
	virtual void die();
	virtual void resize(float scale);
	virtual void rotate(Direction direction);
	void change_directions(Direction direction);
	sf::Vector2f peer;
	sf::Color color;
	Direction primary_dir = IDLE, secondary_dir = IDLE;
	Direction last_direction;
	const sf::Vector2f start_peer;
	static sf::Vector2f object_start_pos;
	std::vector<BodyPart> body;
protected:
	std::chrono::high_resolution_clock::time_point last_animation;
	int rotation;
};

