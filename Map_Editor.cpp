#include "Map_Editor.h"
#include <iostream>

static sf::Vector2f objects_starting_positions = { 2.25f * Options::object_size, 1.25f * Options::object_size };

Map_Editor::Map_Editor(sf::RenderWindow& window, uint16_t x, uint16_t y)
	:window(window)
{
	local_bounds.setFillColor(sf::Color::Black);
	local_bounds.setOutlineColor(sf::Color::Blue);
	local_bounds.setOutlineThickness(2.f);
	local_bounds.setPosition(objects_starting_positions.x - 0.25 * Options::object_size, objects_starting_positions.y - 0.25 * Options::object_size);
	resize_map(x, y);
}

void Map_Editor::resize_map(uint16_t x, uint16_t y)
{
	if (x == 0) x = 1;
	if (y == 0) y = 1;
	for (std::vector<Object>& vec : objects)
		vec.resize(y, NONE);
	objects.resize(x, std::vector<Object>(y, NONE));
	float size = Options::object_size / 2;
	local_bounds.setSize({ static_cast<float>(x) * size + 0, static_cast<float>(y) * size + 0 });
	lines.reserve((x + 1) * (y + 1) * 2);
}

sf::Vector2i Map_Editor::get_tile_pos(sf::Vector2f pos)
{
	pos -= local_bounds.getPosition();

	if (pos.x < 0 || pos.y < 0 || pos.x >= local_bounds.getSize().x || pos.y >= local_bounds.getSize().y)
		return { -1, -1 };

	float x_pos = pos.x * 2 / Options::object_size;
	float y_pos = pos.y * 2 / Options::object_size;

	uint16_t x = static_cast<uint16_t>(x_pos) * Options::object_size / 2 + Options::object_size / 4;
	uint16_t y = static_cast<uint16_t>(y_pos) * Options::object_size / 2 + Options::object_size / 4;

	return { x, y };	
}

void Map_Editor::draw()
{
	sf::CircleShape peer(1.f, 24);
	peer.setFillColor(sf::Color::White);
	peer.setOrigin((peer.getRadius() / 2), (peer.getRadius() / 2));

	sf::CircleShape coin(5.f, 24);
	coin.setFillColor(sf::Color::Yellow);
	coin.setOrigin((coin.getRadius() / 2), (coin.getRadius() / 2));

	window.draw(local_bounds);
	//float start = 2.25 * Options::object_size;
	float index = Options::object_size / 2;
	float x = 0;
	for (auto& row : objects)
	{
		float y = 0;
		for (Object object : row)
		{
			if (object == NONE) 
			{
				//peer.setPosition(start + x * index, start - Options::object_size + y * index);
				peer.setPosition(objects_starting_positions.x + x * index, objects_starting_positions.y + y * index);
				window.draw(peer);
			}
			else if (object == COIN)
			{
				coin.setPosition(objects_starting_positions.x + x * index, objects_starting_positions.y + y * index);
				window.draw(coin);
			}
			y++;
		}
		x++;
	}
	for (const Line& line : lines)
	{
		window.draw(line.vertices, 2, sf::Lines);
	}
}

void Map_Editor::update_buffer()
{
	sf::Vector2i diff = path_buffer.pos - mouse_pos;
	if (std::abs(diff.x) > std::abs(diff.y))
	{
		if (diff.x > 0)
		{
			path_buffer.direction = RIGHT;
			path_buffer.length = diff.x / Options::object_size * 2;
		}
		else
		{
			path_buffer.direction = LEFT;
			path_buffer.length = -diff.x / Options::object_size * 2;
		}
	}
	else
	{
		if (diff.y > 0)
		{
			path_buffer.direction = DOWN;
			path_buffer.length = diff.y / Options::object_size * 2;
		}
		else
		{
			path_buffer.direction = UP;
			path_buffer.length = -diff.y / Options::object_size * 2;
		}
	}
}

Line Map_Editor::get_line_from_buffer(sf::Color color)
{
	Line line;
	line.vertices[0] = sf::Vertex(static_cast<sf::Vector2f>(path_buffer.pos) + local_bounds.getPosition(), color);
	switch (path_buffer.direction)
	{
	case LEFT:
	{
		line.vertices[1] = sf::Vertex(sf::Vector2f(path_buffer.pos.x + Options::object_size * path_buffer.length / 2, path_buffer.pos.y), color);
		break;
	}
	case RIGHT:
	{
		line.vertices[1] = sf::Vertex(sf::Vector2f(path_buffer.pos.x - Options::object_size * path_buffer.length / 2, path_buffer.pos.y), color);
		break;
	}
	case UP:
	{
		line.vertices[1] = sf::Vertex(sf::Vector2f(path_buffer.pos.x, path_buffer.pos.y + Options::object_size * path_buffer.length / 2), color);
		break;
	}
	case DOWN:
	{
		line.vertices[1] = sf::Vertex(sf::Vector2f(path_buffer.pos.x, path_buffer.pos.y - Options::object_size * path_buffer.length / 2), color);
		break;
	}
	}
	line.vertices[1].position += local_bounds.getPosition();
	return line;
}

std::vector<Line> Map_Editor::get_lines_from_buffer(sf::Color color)
{
	sf::Vector2f index = { 0, 0 };
	switch (path_buffer.direction)
	{
	case LEFT:
	{
		index.x = -Options::object_size / 2;
		break;
	}
	case RIGHT:
	{
		index.x = Options::object_size / 2;
		break;
	}		
	case DOWN:
	{
		index.y = Options::object_size / 2;
		break;
	}
	case UP:
	{
		index.y = -Options::object_size / 2;
		break;
	}
	}
	std::vector<Line> lines(path_buffer.length, Line());
	for (float i = 0; i < path_buffer.length; i++)
	{
		lines[i].vertices[0] = sf::Vertex(static_cast<sf::Vector2f>(path_buffer.pos) - i * index + local_bounds.getPosition(), color);
		lines[i].vertices[1] = sf::Vertex(static_cast<sf::Vector2f>(path_buffer.pos) - (i + 1) * index + local_bounds.getPosition(), color);
	}
	return lines;
}

void Map_Editor::run()
{
	if (!window.isOpen())
		return;
	if (!window.hasFocus())
		return;
	sf::Event event;
	if (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
		{
			window.close();
			break;
		}
		case sf::Event::MouseButtonPressed:
		{
			path_buffer = Path_Buffer(get_tile_pos(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))));
			if (event.mouseButton.button == sf::Mouse::Left)
				place_object = true;
			else
				place_object = false;
			break;
		}
		case sf::Event::MouseButtonReleased:
		{
			std::vector<Line> vec = get_lines_from_buffer(sf::Color::White);
			if (place_object)
			{
				for (Line& line : vec)
					lines.insert(line);
			}
			else
			{
				for (Line& line : vec)
					lines.erase(line);
			}
			path_buffer.pos.x = -1;
			break;
		}
		case sf::Event::KeyPressed:
		{
			place_object = true;
			if (event.key.code == sf::Keyboard::C)
				object = COIN;
			else if (event.key.code == sf::Keyboard::D)
				object = NONE;
			break;
		}
		case sf::Event::KeyReleased:
		{
			place_object = false;
			break;
		}
		}
	}
	window.clear();
	draw();
	if (path_buffer.pos.x != -1)
	{
		mouse_pos = sf::Mouse::getPosition(window) - static_cast<sf::Vector2i>(local_bounds.getPosition());
		update_buffer();
		window.draw(get_line_from_buffer(sf::Color::Red).vertices, 2, sf::Lines);
	}
	window.display();
}//