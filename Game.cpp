#include "Game.h"

const sf::Vector2f start_pos(100.f, 100.f);

Game::Game(sf::RenderWindow& window, const std::vector<Line>& walls, std::vector<Entity*>& entities, const std::vector<std::vector<Peer>>& peers)
	:window(window), walls(walls), entities(entities) 
{
	int rows = peers.size();
	int cols = (rows > 0) ? peers[0].size() : 0;

	this->peers.resize(cols, std::vector<Peer>(rows));
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			this->peers[j][i] = peers[i][j];
		}
	}
	for (Entity* entity : entities)
	{
		entity->resize(Shapes::get_scale());
	}
}

sf::Vector2i Game::get_peer(sf::Vector2f pos)
{
	int X = static_cast<int>(pos.x);
	int Y = static_cast<int>(pos.y);

	float x = pos.x - X;
	float y = pos.y - Y;
	
	if (x < Options::tolerance)
		X--;
	else if (x > 1 - Options::tolerance)
		X++;

	if (y < Options::tolerance)
		Y--;
	else if (y > 1 - Options::tolerance)
		Y++;

	return { X, Y };
}

void Game::run()
{
	Entity::object_start_pos = start_pos;
	for (Entity* entity : entities)
		if (try_move(*entity, entity->primary_dir, Options::game_speed / Options::fps))
			entity->rotate(entity->primary_dir);
		else if (try_move(*entity, entity->secondary_dir, Options::game_speed / Options::fps))
			entity->rotate(entity->secondary_dir);
	check_collisons();
	draw();
}

void Game::check_collisons()
{
	for (int i = 0; i < entities.size(); i++)
	{
		for (int j = i + 1; j < entities.size(); j++)
		{
			if ((std::abs(entities[i]->peer.x - entities[j]->peer.x) < Options::hitbox) && (std::abs(entities[i]->peer.y - entities[j]->peer.y) < Options::hitbox))
			{
				entities[j]->colision(*entities[i]);
				entities[i]->colision(*entities[j]);
			}
		}
	}
}

Object Game::eat(sf::Vector2f position)
{
	sf::Vector2f decimal = position - static_cast<sf::Vector2f>(static_cast<sf::Vector2i>(position));
	sf::Vector2i pos = static_cast<sf::Vector2i>(position);

	if (decimal.x > 0.8)
		pos.x++;
	else if(decimal.x > 0.2)
		return NONE;

	if (decimal.y > 0.8)
		pos.y++;
	else if (decimal.y > 0.2)
		return NONE;

	if (pos.y > peers.size() || pos.x > peers[0].size())
		return NONE;
	Object object = peers[pos.y][pos.x].object;
	peers[pos.y][pos.x].object = NONE;
	return object;
}

bool Game::try_move(Entity& entity, Direction direction, float distance)
{
	float x_decimal = entity.peer.x - static_cast<int>(entity.peer.x);
	float y_decimal = entity.peer.y - static_cast<int>(entity.peer.y);
	int x = entity.peer.x;
	int y = entity.peer.y;
	switch (direction)
	{
	case UP:
	{
		if (x_decimal > 0.9)
			x++;
		else if (x_decimal > 0.1)
			return false;
		if (y_decimal - distance < 0)
		{
			if (peers[y][x].up)
				entity.peer.y -= distance;
			else
			{
				entity.peer.y = y;
				return false;
			}
		}
		else if (peers[y + 1][x].up)
			entity.peer.y -= distance;
		else
			return false;
		entity.peer.x = x;
		break;
	}
	case LEFT:
	{
		if (y_decimal > 0.9)
			y++;
		else if (y_decimal > 0.1)
			return false;
		if (x_decimal - distance < 0)
		{
			if (peers[y][x].left)
				entity.peer.x -= distance;
			else
			{
				entity.peer.x = x;
				return false;
			}
		}
		else if (peers[y][x + 1].left)
			entity.peer.x -= distance;
		else
			return false;
		entity.peer.y = y;
		break;
	}
	case DOWN:
	{
		if (x_decimal > 0.9)
			x++;
		else if (x_decimal > 0.1)
			return false;
		if (y_decimal + distance > 1)
		{
			if (peers[y + 1][x].down)
				entity.peer.y += distance;
			else
			{
				entity.peer.y = y + 1;
				return false;
			}
		}
		else if (peers[y][x].down)
			entity.peer.y += distance;
		else
			return false;
		entity.peer.x = x;
		break;
	}
	case RIGHT:
	{
		if (y_decimal > 0.9)
			y++;
		else if (y_decimal > 0.1)
			return false;
		if (x_decimal + distance > 1)
		{
			if (peers[y][x + 1].right)
				entity.peer.x += distance;
			else
			{
				entity.peer.x = x + 1;
				return false;
			}
		}
		else if (peers[y][x].right)
			entity.peer.x += distance;
		else
			return false;
		entity.peer.y = y;
		break;
	}
	case IDLE:
		break;
	}
	return true;
}

void Game::draw()
{
	draw_entities();
	draw_walls();
	draw_objects();
}

void Game::draw_walls()
{
	sf::Vertex Vertices[] =
	{
		sf::Vertex({0, 0}, sf::Color::Blue),
		sf::Vertex({0, 0}, sf::Color::Blue)
	};
	for (const Line& line : walls)
	{
		Vertices[0].position = static_cast<sf::Vector2f>(line.A) * Options::object_size / 2.f + start_pos;
		Vertices[1].position = static_cast<sf::Vector2f>(line.B) * Options::object_size / 2.f + start_pos;
		window.draw(Vertices, 2, sf::Lines);
	}
}

void Game::draw_objects()
{
	sf::CircleShape coin(Options::object_size / 15, Options::num_circle_points);
	coin.setFillColor(sf::Color::Yellow);
	coin.setOrigin((coin.getRadius()), (coin.getRadius()));
	for (int x = 0; x < peers[0].size(); x++)
	{
		for (int y = 0; y < peers.size(); y++)
			if (peers[y][x].object == COIN)
			{
				coin.setPosition(static_cast<sf::Vector2f>(start_pos + Options::object_size / 2 * sf::Vector2f(x, y)));
				window.draw(coin);
			}
		for (int y = 0; y < peers.size(); y++)
			if (peers[y][x].object == POWERUP)
			{
				coin.setRadius(Options::object_size / 6);
				coin.setOrigin((coin.getRadius()), (coin.getRadius()));
				coin.setPosition(static_cast<sf::Vector2f>(start_pos + Options::object_size / 2 * sf::Vector2f(x, y)));
				window.draw(coin);
				coin.setRadius(Options::object_size / 15);
				coin.setOrigin((coin.getRadius()), (coin.getRadius()));
			}
	}
}

void Game::draw_entities()
{
	for (Entity* entity : entities)
		entity->draw(window);
}