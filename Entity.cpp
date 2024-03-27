#include "Entity.h"

Entity::Entity(sf::Color color, const sf::Vector2f start_pos, std::chrono::high_resolution_clock::time_point game_start)
	:color(color), last_animation(game_start), start_peer(start_pos), peer(start_pos) 
{
}

sf::Vector2f Entity::object_start_pos;


void Entity::draw(sf::RenderWindow& window)
{
	auto current_time = std::chrono::high_resolution_clock::now();
	auto time_passed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_animation);
	last_animation = current_time;
	animate(time_passed);
	for (BodyPart& part : body)
	{
		part.draw(window, object_start_pos + peer * Options::object_size / 2.f, color, rotation);
	}
}

void Entity::die()
{
	change_directions(IDLE);
	peer = start_peer;
}

void Entity::change_directions(Direction direction)
{
	switch (direction)
	{
	case UP:
		if (primary_dir == UP || primary_dir == DOWN)
			primary_dir = UP;
		else
		{
			secondary_dir = primary_dir;
			primary_dir = UP;
		}
		break;
	case DOWN:
		if (primary_dir == UP || primary_dir == DOWN)
			primary_dir = DOWN;
		else
		{
			secondary_dir = primary_dir;
			primary_dir = DOWN;
		}
		break;
	case LEFT:
		if (primary_dir == LEFT || primary_dir == RIGHT)
			primary_dir = LEFT;
		else
		{
			secondary_dir = primary_dir;
			primary_dir = LEFT;
		}
		break;
	case RIGHT:
		if (primary_dir == LEFT || primary_dir == RIGHT)
			primary_dir = RIGHT;
		else
		{
			secondary_dir = primary_dir;
			primary_dir = RIGHT;
		}
		break;
	case IDLE:
		secondary_dir = primary_dir;
		primary_dir = IDLE;
		break;
	default:
		break;
	}
}

void Entity::resize(float scale)
{
	for (BodyPart& part : body)
		part.resize(scale);
}

void Entity::rotate(Direction direction)
{
	switch (direction)
	{
	case UP:
		rotation = 270;
		break;
	case DOWN:
		rotation = 90;
		break;
	case LEFT:
		rotation = 180;
		break;
	case RIGHT:
		rotation = 0;
		break;
	default:
		break;
	}
}
