#include "Ghost.h"

Ghost::Ghost(sf::Color color, std::chrono::high_resolution_clock::time_point game_start)
	:Entity(color, game_start) 
{
	body = create_body();
}
std::vector<BodyPart> Ghost::create_body()
{
	static std::vector<BodyPart> body
	{
		BodyPart(Shapes::get(GHOST_BODY), { 0, 0 }),
		BodyPart(Shapes::get(GHOST_LEGS1), { 0, Options::object_size }),
		BodyPart(Shapes::get(GHOST_LEGS2), { 0, Options::object_size }),
		BodyPart(Shapes::get(GHOST_LEGS1), { 2 * Options::object_size / 3, Options::object_size }),
		BodyPart(Shapes::get(GHOST_LEGS2), { 2 * Options::object_size / 3, Options::object_size }),
		BodyPart(Shapes::get(GHOST_LEGS1), { -2 * Options::object_size / 3, Options::object_size }),
		BodyPart(Shapes::get(GHOST_LEGS2), { -2 * Options::object_size / 3, Options::object_size })
	};
	return body;
}
#include <iostream>
void Ghost::animate(std::chrono::milliseconds time_passed)
{
	float distance_ratio = static_cast<float>(time_passed.count()) / static_cast<float>(Options::animation_speed_miliseconds);
	float distance = static_cast<float>(Options::object_size * distance_ratio  / 3);
	if (switch_animation)
	{
		body[2].local_pos.x += distance;
		body[4].local_pos.x += distance;
		body[6].local_pos.x += distance;

		body[1].local_pos.x -= distance;
		body[3].local_pos.x -= distance;
		body[5].local_pos.x -= distance; 
		
		if (body[6].local_pos.x >= -1 * Options::object_size / 3)
		{
			static auto now = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::high_resolution_clock::now() - now;
			//std::cout << " Ghost: " << duration.count() / 1000000 << std::endl;
			now = std::chrono::high_resolution_clock::now();
			switch_animation = false;
		}
	}
	else
	{
		body[2].local_pos.x -= distance;
		body[4].local_pos.x -= distance;
		body[6].local_pos.x -= distance;

		body[1].local_pos.x += distance;
		body[3].local_pos.x += distance;
		body[5].local_pos.x += distance; 
		
		if (body[6].local_pos.x <= -2 * Options::object_size / 3)
			switch_animation = true;
	}
}