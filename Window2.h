#pragma once
#include <SFML/Graphics.hpp>
#include "Options.h"

class Window
{
public:
	Window(const std::string& name);
	Window(sf::Vector2i size, const std::string& name);
	//Window(const Window& other);
	//Window& operator=(const Window& other); 
	void create(sf::Vector2i size, const std::string& name);
	void create(const std::string& name);
	void run(const sf::Event& event);
	sf::RenderWindow window;
	void move(sf::Vector2i direction);
private:
	void resize();
};

