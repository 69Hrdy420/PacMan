#include "Entity.h"

Entity::Entity(sf::Color color, std::chrono::high_resolution_clock::time_point game_start)
	:color(color), last_animation(game_start) {}

void Entity::draw(sf::RenderWindow& window)
{
	auto current_time = std::chrono::high_resolution_clock::now();
	auto time_passed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_animation);
	last_animation = current_time;
	animate(time_passed);
	for (BodyPart& part : body)
		part.draw(window, pos, color);
}

