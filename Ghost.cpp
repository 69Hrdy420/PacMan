#include "Ghost.h"

Ghost::Ghost(sf::Color color, const sf::Vector2f start_pos, std::chrono::high_resolution_clock::time_point game_start)
	:Entity(color, start_pos, game_start)
{
	body = create_body(1);
}
std::vector<BodyPart> Ghost::create_body(float scale)
{
	static std::vector<BodyPart> body
	{
		BodyPart(Shapes::get(GHOST_BODY), { 0, 0 }),
		BodyPart(Shapes::get(GHOST_LEGS1), { 0, Options::object_size}),
		BodyPart(Shapes::get(GHOST_LEGS2), { 0, Options::object_size}),
		BodyPart(Shapes::get(GHOST_LEGS1), { 2.f * Options::object_size * scale / 3.f, Options::object_size }),
		BodyPart(Shapes::get(GHOST_LEGS2), { 2.f * Options::object_size * scale / 3.f, Options::object_size }),
		BodyPart(Shapes::get(GHOST_LEGS1), { -2.f * Options::object_size * scale / 3.f, Options::object_size }),
		BodyPart(Shapes::get(GHOST_LEGS2), { -2.f * Options::object_size * scale / 3.f, Options::object_size })
	};
	return body;
}

void Ghost::animate(std::chrono::milliseconds time_passed)
{
	float distance_ratio = static_cast<float>(time_passed.count()) / static_cast<float>(Options::animation_speed_miliseconds / 0.4);
	float distance = static_cast<float>(Options::object_size * distance_ratio / 3);
	float scale = Shapes::get_scale();
	if (switch_animation)
	{
		body[2].local_pos.x += distance;
		body[4].local_pos.x += distance;
		body[6].local_pos.x += distance;

		body[1].local_pos.x -= distance;
		body[3].local_pos.x -= distance;
		body[5].local_pos.x -= distance; 
		
		if (body[6].local_pos.x >= -scale * Options::object_size / 3)
		{
			static auto now = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::high_resolution_clock::now() - now;
			now = std::chrono::high_resolution_clock::now();
			switch_animation = false;
			//std::cin.get();
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
		
		if (body[6].local_pos.x <= -2 * scale * Options::object_size / 3)
		{
			switch_animation = true;
			//std::cin.get();
		}
	}
}

void Ghost::rotate(Direction direction) {}

void Ghost::resize(float scale)
{
	body = create_body(scale);

	//int dist_x = body[0].local_pos.x;
	for (BodyPart& part : body)
	{
		part.local_pos.x *= scale;
		part.local_pos.y *= scale;
	}
	//body[2].local_pos.x += 1 * scale * body[2].local_pos.x;
	//body[4].local_pos.x += 1 * scale * body[4].local_pos.x;
	//body[6].local_pos.x += 1 * scale * body[6].local_pos.x;

	//body[1].local_pos.x += 1000;
	//body[3].local_pos.x += 1000;
	//body[5].local_pos.x += 1000;
}