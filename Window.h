#pragma once
#include <SFML/Graphics.hpp>
#include "Options.h"


class Windows
{
public:
	~Windows();
	static sf::RenderWindow* get_new();
	static void remove(sf::RenderWindow& window);
	static void deinitialize();
	//static void resize(const sf::RenderWindow& window, sf::Vector2i size);
private:
	struct List
	{
		List();			
		sf::RenderWindow window;
		List* next = nullptr;
	};
	static List* head;
};


//extern sf::RenderWindow window;
