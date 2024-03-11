#include "Window.h"
#include <iostream>

Windows::List* Windows::head = nullptr;

Windows::List::List()
	:window(sf::VideoMode(Options::window_size_x, Options::window_size_y), "Pac-Man") {}

sf::RenderWindow* Windows::get_new()
{
	std::cout << "Window created" << std::endl;
	List* list = new List;
	list->next = head;
	head = list;
	return &(list->window);
}

void Windows::remove(sf::RenderWindow& window)
{
	std::cout << "Window removed" << std::endl;
	List* previous = head;
	if (!previous)
		return;
	List* current = head->next;
	while (current)
	{
		if (&(current->window) == &window)
		{
			previous->next = current->next;
			delete current;
		}
		previous = current;
		current = current->next;
	}
}

void Windows::deinitialize()
{
	List* previous = head;
	if (!previous)
		return;
	List* current = head->next;
	while (current)
	{
		delete previous;
		std::cout << "Window removed" << std::endl;
		previous = current;
		current = current->next;
	}
	delete previous;
	std::cout << "Window removed" << std::endl;
}

Windows::~Windows()
{
	deinitialize();
}


//sf::RenderWindow window(sf::VideoMode(1200, 1000), "Pac-Man");
