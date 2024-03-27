#include "Map_Editor2.h"
#include <iostream>
#include <functional>

static const sf::Vector2f objects_starting_positions = { 200.f , 100.f };

Map_Editor::Map_Editor(Window& window, uint16_t x, uint16_t y)
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
	window.window.draw(local_bounds);
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
		window.window.draw(vertexes, 2, sf::Lines);
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
	window.window.draw(vertexes, 2, sf::Lines);
}

void Map_Editor::draw_objects()
{
	sf::CircleShape peer(Options::object_size / 60, Options::num_circle_points);
	peer.setFillColor(sf::Color::White);
	peer.setOrigin((peer.getRadius()), (peer.getRadius()));

	sf::CircleShape coin(Options::object_size / 15, Options::num_circle_points);
	coin.setFillColor(sf::Color::Yellow);
	coin.setOrigin((coin.getRadius()), (coin.getRadius()));

	for (int x = 0; x < objects.size(); x++)
	{
		for (int y = 0; y < objects[0].size(); y++)
			if (objects[x][y] == NONE)
			{
				peer.setPosition(static_cast<sf::Vector2f>(get_peer_pos({ x, y })));
				window.window.draw(peer);
			}
			else if (objects[x][y] == COIN)
			{
				coin.setPosition(static_cast<sf::Vector2f>(get_peer_pos({ x, y })));
				window.window.draw(coin);
			}
			else if (objects[x][y] == STARTPOS)
			{
				coin.setFillColor(sf::Color::Green);
				coin.setRadius(Options::object_size / 7.5);
				coin.setOrigin((coin.getRadius()), (coin.getRadius()));
				coin.setPosition(static_cast<sf::Vector2f>(get_peer_pos({ x, y })));
				window.window.draw(coin);
				coin.setFillColor(sf::Color::Yellow);
				coin.setRadius(Options::object_size / 15);
				coin.setOrigin((coin.getRadius()), (coin.getRadius()));
			}
			else if (objects[x][y] == GHOST1)
			{
				coin.setFillColor(sf::Color::Red);
				coin.setRadius(Options::object_size / 7.5);
				coin.setOrigin((coin.getRadius()), (coin.getRadius()));
				coin.setPosition(static_cast<sf::Vector2f>(get_peer_pos({ x, y })));
				window.window.draw(coin);
				coin.setFillColor(sf::Color::Yellow);
				coin.setRadius(Options::object_size / 15);
				coin.setOrigin((coin.getRadius()), (coin.getRadius()));
			}
			else if (objects[x][y] == POWERUP)
			{
				coin.setRadius(Options::object_size / 7.5);
				coin.setOrigin((coin.getRadius()), (coin.getRadius()));
				coin.setPosition(static_cast<sf::Vector2f>(get_peer_pos({ x, y })));
				window.window.draw(coin);
				coin.setRadius(Options::object_size / 15);
				coin.setOrigin((coin.getRadius()), (coin.getRadius()));
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
			window.window.draw(vertexes, 2, sf::Lines);
			vertexes[0].position = objects_starting_positions + i * sf::Vector2f(0, Options::object_size / 2) + sf::Vector2f((objects.size() - 1) * Options::object_size / 2, 0);
			vertexes[1].position = vertexes[0].position + sf::Vector2f(Options::object_size / 4, 0);
			window.window.draw(vertexes, 2, sf::Lines);
		}
	for (float i = 0; i < vertical_bridges.size(); i++)
		if (vertical_bridges[i] == true)
		{
			vertexes[0].position = objects_starting_positions + i * sf::Vector2f(Options::object_size / 2, 0);
			vertexes[1].position = vertexes[0].position - sf::Vector2f(0, Options::object_size / 4);
			window.window.draw(vertexes, 2, sf::Lines);
			vertexes[0].position = objects_starting_positions + i * sf::Vector2f(Options::object_size / 2, 0) + sf::Vector2f(0, (objects[0].size() - 1) * Options::object_size / 2);
			vertexes[1].position = vertexes[0].position + sf::Vector2f(0, Options::object_size / 4);
			window.window.draw(vertexes, 2, sf::Lines);
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
	case sf::Keyboard::B:
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
		Window game_window("PacMan");
		game_window.window.setFramerateLimit(Options::fps);
		Map_Loader loader(window, get_paths(), get_objects(), {}, {});
		std::vector<Line> walls = loader.get_walls();
		std::vector<std::vector<Peer>> peers = loader.get_peers();
		std::vector<PacMan*> pacmans;
		std::vector<Entity*> entities;
		for (int i = 0; i < objects.size(); i++)
			for (int j = 0; j < objects[0].size(); j++)
				if (objects[i][j] == STARTPOS)
				{
					pacmans.emplace_back(new PacMan(sf::Color::Yellow, sf::Vector2f(i, j), std::chrono::high_resolution_clock::now()));
					entities.push_back(pacmans.back());
				}
				else if (objects[i][j] == GHOST1)
					entities.push_back(new Ghost(sf::Color::Red, sf::Vector2f(i, j), std::chrono::high_resolution_clock::now()));
		Game game(game_window.window, walls, entities, peers);
		Input input;
		input.get_input1(*entities[0]);
		input.get_input2(*entities[1]);
		for (;;)
		{
			game_window.window.clear();
			sf::Event event;
			if (game_window.window.pollEvent(event))
			{
				game_window.run(event);
				if (event.type == sf::Event::KeyPressed)
				{
					input.run(event.key.code);
				}
			}
			game.run();
			for (PacMan* pacman : pacmans)
				pacman->eat(game.eat(pacman->peer));
			game_window.window.display();
			if (!game_window.window.isOpen())
				break;
		}
		for (Entity* entity : entities)
			delete entity;
		break;
	}
	case sf::Keyboard::P:
	{
		sf::Vector2i pos = get_peer(sf::Mouse::getPosition(window.window));
		if (pos.x >= 0 && pos.y >= 0 && pos.x < objects.size() && pos.y < objects[0].size())
			objects[pos.x][pos.y] = STARTPOS;
		break;
	}
	case sf::Keyboard::G:
	{
		sf::Vector2i pos = get_peer(sf::Mouse::getPosition(window.window));
		if (pos.x >= 0 && pos.y >= 0 && pos.x < objects.size() && pos.y < objects[0].size())
			objects[pos.x][pos.y] = GHOST1;
		break;
	}
	case sf::Keyboard::L:
	{
		Window w("Map Loader");
		Map_Loader loader(w, lines, objects, load_bridges(horizontal_bridges), load_bridges(vertical_bridges));
		for (;;)
		{
			sf::Event event;
			w.window.pollEvent(event);
			w.window.clear();
			w.run(event);
			if (!loader.run(event))
				break;
			w.window.display();
		}
		break;
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
	//for (bool b : horizontal_bridges)
	//	std::cout << b;
	//std::cout << std::endl;
	//for (bool b : vertical_bridges)
	//	std::cout << b;
	//std::cout << std::endl;
	for (const Line& line : lines)
		std::cout << line.A.x << " " << line.A.y << " " << line.B.x << " " << line.B.y << " ";
	std::cout << std::endl << "--------------------------------------------------------" << std::endl;
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
	if (data.size() < data[0] * data[1] + data[0] + data[1])
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

bool Map_Editor::run()
{
	if (!window.window.isOpen())
		return false;
	if (!window.window.hasFocus())
		return true;
	sf::Event event;
	if (window.window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
		{
			return false;
		}
		case sf::Event::MouseButtonPressed:
		{
			path_buffer.pos = get_peer_pos(get_peer(sf::Mouse::getPosition(window.window)));
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
		update_buffer(sf::Mouse::getPosition(window.window));
	else if (place_object)
	{
		sf::Vector2i pos = get_peer(sf::Mouse::getPosition(window.window));
		if (pos.x >= 0 && pos.y >= 0 && pos.x < objects.size() && pos.y < objects[0].size())
			objects[pos.x][pos.y] = object;
	}
	draw();
	return true;
}

std::vector<std::vector<Object>> Map_Editor::get_objects()
{
	return objects;
}

std::unordered_set<Line, LineHash> Map_Editor::get_paths()
{
	return lines;
}