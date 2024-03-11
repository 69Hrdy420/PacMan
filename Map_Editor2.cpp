#include "Map_Editor2.h"
#include <iostream>

static const sf::Vector2f objects_starting_positions = { 200.f , 100.f };

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
	if (x == 0)
		x = 1;
	if (y == 0)
		y = 1;
	for (std::vector<Object>& vec : objects)
		vec.resize(y, NONE);
	objects.resize(x, std::vector<Object>(y, NONE));
	local_bounds.setSize({ static_cast<float>(x) * Options::object_size / 2.f, static_cast<float>(y) * Options::object_size / 2.f });
	std::vector<Line*> erase;
	erase.reserve(lines.size());
	for (const Line& line : lines)
	{
		if (line.A.x >= x || line.A.y >= y || line.B.x >= x || line.B.y >= y)
			erase.emplace_back(const_cast<Line*>(&line));
	}
	for (Line* line : erase)
	{
		lines.erase(*line);
	}
	lines.reserve((x + 1) * (y + 1) * 2);
	horizontal_bridges.resize(y, false);
	vertical_bridges.resize(x, false);
}

sf::Vector2i Map_Editor::get_peer(sf::Vector2i pos)
{
	pos -= static_cast<sf::Vector2i>(local_bounds.getPosition());
	sf::Vector2f tile_size = { local_bounds.getSize().x / objects.size(), local_bounds.getSize().y / objects[0].size() };
	sf::Vector2i result =
	{
		static_cast<int>(pos.x / tile_size.x),
		static_cast<int>(pos.y / tile_size.y)
	};
	return result;
}

sf::Vector2i Map_Editor::get_peer_pos(sf::Vector2i peer) 
{
	return static_cast<sf::Vector2i>(objects_starting_positions + static_cast<sf::Vector2f>(peer) * Options::object_size / 2.f);
}

void Map_Editor::draw()
{
	window.draw(local_bounds);
	draw_paths();
	draw_objects();
	draw_bridges();
}

void Map_Editor::draw_paths()
{
	sf::Vertex vertexes[2] = { sf::Vertex(sf::Vector2f(0.f, 0.f), sf::Color::White),
								sf::Vertex(sf::Vector2f(0.f, 0.f), sf::Color::White) };
	for (const Line& line : lines)
	{
		vertexes[0].position = static_cast<sf::Vector2f>(line.A) * Options::object_size / 2.f + objects_starting_positions;
		vertexes[1].position = static_cast<sf::Vector2f>(line.B) * Options::object_size / 2.f + objects_starting_positions;
		window.draw(vertexes, 2, sf::Lines);
	}
	if (path_buffer.is_creating)
	{
		vertexes[0].color = sf::Color::Green;
		vertexes[1].color = sf::Color::Green;
	}
	else
	{
		vertexes[0].color = sf::Color::Red;
		vertexes[1].color = sf::Color::Red;
	}
	vertexes[0].position = static_cast<sf::Vector2f>(path_buffer.pos);
	if (path_buffer.is_horizontal)
	{
		vertexes[1].position.x = path_buffer.pos.x + Options::object_size / 2 * path_buffer.length;
		vertexes[1].position.y = path_buffer.pos.y;
	}
	else
	{
		vertexes[1].position.x = path_buffer.pos.x;
		vertexes[1].position.y = path_buffer.pos.y + Options::object_size / 2 * path_buffer.length;
	}
	window.draw(vertexes, 2, sf::Lines);
}

void Map_Editor::draw_objects()
{
	sf::CircleShape peer(1.f, 24);
	peer.setFillColor(sf::Color::White);
	peer.setOrigin((peer.getRadius()), (peer.getRadius()));

	sf::CircleShape coin(5.f, 24);
	coin.setFillColor(sf::Color::Yellow);
	coin.setOrigin((coin.getRadius()), (coin.getRadius()));

	for (int x = 0; x < objects.size(); x++)
	{
		for (int y = 0; y < objects[0].size(); y++)
			if (objects[x][y] == NONE)
			{
				peer.setPosition(static_cast<sf::Vector2f>(get_peer_pos({ x, y })));
				window.draw(peer);
			}
			else if (objects[x][y] == COIN)
			{
				coin.setPosition(static_cast<sf::Vector2f>(get_peer_pos({ x, y })));
				window.draw(coin);
			}
	}
}

void Map_Editor::draw_bridges()
{
	sf::Vertex vertexes[2] = { sf::Vertex(sf::Vector2f(0.f, 0.f), sf::Color::White),
								sf::Vertex(sf::Vector2f(0.f, 0.f), sf::Color::White) };
	for (float i = 0; i < horizontal_bridges.size(); i++)
		if (horizontal_bridges[i] == true)
		{
			vertexes[0].position = objects_starting_positions + i * sf::Vector2f(0, Options::object_size / 2);
			vertexes[1].position = vertexes[0].position - sf::Vector2f(Options::object_size / 4, 0);
			window.draw(vertexes, 2, sf::Lines);
			vertexes[0].position = objects_starting_positions + i * sf::Vector2f(0, Options::object_size / 2) + sf::Vector2f((objects.size() - 1) * Options::object_size / 2, 0);
			vertexes[1].position = vertexes[0].position + sf::Vector2f(Options::object_size / 4, 0);
			window.draw(vertexes, 2, sf::Lines);
		}
	for (float i = 0; i < vertical_bridges.size(); i++)
		if (vertical_bridges[i] == true)
		{
			vertexes[0].position = objects_starting_positions + i * sf::Vector2f(Options::object_size / 2, 0);
			vertexes[1].position = vertexes[0].position - sf::Vector2f(0, Options::object_size / 4);
			window.draw(vertexes, 2, sf::Lines);
			vertexes[0].position = objects_starting_positions + i * sf::Vector2f(Options::object_size / 2, 0) + sf::Vector2f(0, (objects[0].size() - 1) * Options::object_size / 2);
			vertexes[1].position = vertexes[0].position + sf::Vector2f(0, Options::object_size / 4);
			window.draw(vertexes, 2, sf::Lines);
		}
}

void Map_Editor::update_buffer(sf::Vector2i mouse_pos)
{
	sf::Vector2i diff = path_buffer.pos - mouse_pos;
	if (std::abs(diff.x) > std::abs(diff.y))
	{
		path_buffer.is_horizontal = true;
		path_buffer.length = - get_peer(path_buffer.pos).x + get_peer({mouse_pos.x, path_buffer.pos.y}).x;
	}
	else
	{
		path_buffer.is_horizontal = false;
		path_buffer.length = - get_peer(path_buffer.pos).y + get_peer({ path_buffer.pos.x, mouse_pos.y }).y;
	}
}

std::vector<Line> Map_Editor::get_lines_from_buffer()
{
	sf::Vector2i start_pos = get_peer(path_buffer.pos);
	if (start_pos.x < 0 || start_pos.y < 0 || start_pos.x >= objects.size() || start_pos.y >= objects[0].size())
		return std::vector<Line>(0);
	std::vector<Line> lines;
	lines.reserve(std::abs(path_buffer.length));
	if (path_buffer.is_horizontal)
	{
		if (path_buffer.length > 0)
		{
			for (int i = 1; i <= path_buffer.length; i++)
				if (start_pos.x + i == objects.size())
				{
					horizontal_bridges[start_pos.y] = path_buffer.is_creating;
					break;
				}
				else
					lines.emplace_back(Line(start_pos + sf::Vector2i(i - 1, 0), start_pos + sf::Vector2i(i, 0)));
		}
		else
			for (int i = -1; i >= path_buffer.length; i--)
				if (start_pos.x + i < 0)
				{
					horizontal_bridges[start_pos.y] = path_buffer.is_creating;
					break;
				}
				else
					lines.emplace_back(Line(start_pos + sf::Vector2i(i + 1, 0), start_pos + sf::Vector2i(i, 0)));
	}
	else
	{
		if (path_buffer.length > 0)
			for (int i = 1; i <= path_buffer.length; i++)
				if (start_pos.y + i == objects[0].size())
				{
					vertical_bridges[start_pos.x] = path_buffer.is_creating;
					break;
				}
				else
					lines.emplace_back(Line(start_pos + sf::Vector2i(0, i - 1), start_pos + sf::Vector2i(0, i)));
		else
			for (int i = -1; i >= path_buffer.length; i--)
				if (start_pos.y + i < 0)
				{
					vertical_bridges[start_pos.x] = path_buffer.is_creating;
					break;
				}
				else
					lines.emplace_back(Line(start_pos + sf::Vector2i(0, i + 1), start_pos + sf::Vector2i(0, i)));
	}
	lines.shrink_to_fit();
	return lines;
}

std::vector<int> Map_Editor::load_bridges(const std::vector<bool>& bridges)
{
	std::vector<int> result;
	for (int i = 0; i < bridges.size(); i++)
		if (bridges[i])
			result.push_back(i);
	result.shrink_to_fit();
	return result;
}

void Map_Editor::key_pressed(sf::Keyboard::Key key)
{
	switch (key)
	{
	case sf::Keyboard::C:
	{
		place_object = true;
		object = COIN;
		break;
	}
	case sf::Keyboard::D:
	{
		place_object = true;
		object = NONE;
		break;
	}
	case sf::Keyboard::P:
	{
		place_object = true;
		object = POWERUP;
		break;
	}
	case sf::Keyboard::Right:
	{
		resize_map(objects.size() + 1, objects[0].size());
		break;
	}
	case sf::Keyboard::Left:
	{
		resize_map(objects.size() - 1, objects[0].size());
		break;
	}
	case sf::Keyboard::Down:
	{
		resize_map(objects.size(), objects[0].size() + 1);
		break;
	}
	case sf::Keyboard::Up:
	{
		resize_map(objects.size(), objects[0].size() - 1);
		break;
	}
	case sf::Keyboard::S:
	{
		static std::vector<int> saved_data;
		if (saved_data.size() == 0)
		{
			saved_data = save();
			resize_map(0, 0);
		}
		else
			load(saved_data);
		break;
	}
	case sf::Keyboard::L:
	{
		Map_Loader ml(lines, objects, load_bridges(horizontal_bridges), load_bridges(vertical_bridges));
		ml.run();
	}
	default:
		break;
	}
}

void Map_Editor::print()
{
	std::cout << objects.size() << " " << objects[0].size() << std::endl;
	for (const auto& row : objects)
		for (Object object : row)
			std::cout << object << " ";
	for (const Line& line : lines)
		std::cout << line.A.x << " " << line.A.y << " " << line.B.x << " " << line.B.y << " ";
}

std::vector<int> Map_Editor::save()
{
	std::vector<int> data;
	data.reserve(lines.size() * 4 + 2 + objects.size() * objects[0].size());
	data.push_back(objects.size());
	data.push_back(objects[0].size());
	for (const auto& row : objects)
		for (Object object : row)
			data.push_back(object);
	for (const Line& line : lines)
	{
		data.push_back(line.A.x);
		data.push_back(line.A.y);
		data.push_back(line.B.x);
		data.push_back(line.B.y);
	}
	return data;
}

void Map_Editor::load(const std::vector<int>& data)
{
	lines.clear();
	if (data.size() < 2)
		return;
	if (data.size() < data[0] * data[1])
		return;
	resize_map(data[0], data[1]);
	int index = 2;
	for (auto& row : objects)
		for (Object& object : row)
		{
			object = static_cast<Object>(data[index]);
			index++;
		}
	for (index; index < data.size() - 3; index += 4)
		lines.insert(Line(sf::Vector2i(data[index], data[index + 1]), sf::Vector2i(data[index + 2], data[index + 3])));
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
			return;
		}
		case sf::Event::MouseButtonPressed:
		{
			path_buffer.pos = get_peer_pos(get_peer(sf::Mouse::getPosition(window)));
			path_buffer.is_active = true;
			if (event.mouseButton.button == sf::Mouse::Left)
				path_buffer.is_creating = true;
			else
				path_buffer.is_creating = false;
			break;
		}
		case sf::Event::MouseButtonReleased:
		{
			path_buffer.is_active = false;
			std::vector<Line> vec(get_lines_from_buffer());
			if (path_buffer.is_creating)
			{
				for (Line& line : vec)
					lines.insert(line);
			}
			else
			{
				for (Line& line : vec)
					lines.erase(line);
			}
			path_buffer.length = 0;
			break;
		}
		case sf::Event::KeyPressed:
		{
			key_pressed(event.key.code);
			break;
		}
		case sf::Event::KeyReleased:
		{
			place_object = false;
			break;
		}
		case sf::Event::MouseWheelMoved:
		{
			if (event.mouseWheel.delta > 0)
			{
				Options::object_size += 25;
				local_bounds.setSize({ static_cast<float>(objects.size()) * Options::object_size / 2.f, static_cast<float>(objects[0].size()) * Options::object_size / 2.f});
				local_bounds.setPosition(objects_starting_positions.x - 0.25 * Options::object_size, objects_starting_positions.y - 0.25 * Options::object_size);
			}
			else if (event.mouseWheel.delta < 0)
			{
				Options::object_size -= 25;
				local_bounds.setSize({ static_cast<float>(objects.size()) * Options::object_size / 2.f, static_cast<float>(objects[0].size()) * Options::object_size / 2.f });
				local_bounds.setPosition(objects_starting_positions.x - 0.25 * Options::object_size, objects_starting_positions.y - 0.25 * Options::object_size);
			}
			break;
		}
		}
	}
	if (path_buffer.is_active)
		update_buffer(sf::Mouse::getPosition(window));
	else if (place_object)
	{
		sf::Vector2i pos = get_peer(sf::Mouse::getPosition(window));
		if (pos.x >= 0 && pos.y >= 0 && pos.x < objects.size() && pos.y < objects[0].size())
			objects[pos.x][pos.y] = object;
	}
	draw();
}