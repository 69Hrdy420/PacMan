#include "Entity.h"
#include <unordered_map>
#include <functional>


class Input
{
public:
	bool add(Entity& entity, sf::Keyboard::Key left, sf::Keyboard::Key right, sf::Keyboard::Key up, sf::Keyboard::Key down, sf::Keyboard::Key stop);
	void run(sf::Keyboard::Key);
	void remove(sf::Keyboard::Key left, sf::Keyboard::Key right, sf::Keyboard::Key up, sf::Keyboard::Key down, sf::Keyboard::Key stop);
	void remove(const Entity& entity);
	bool get_input1(Entity& entity);
	bool get_input2(Entity& entity);
	void remove_input1();
	void remove_input2();
private:
	std::unordered_map<sf::Keyboard::Key, std::pair<Entity*, std::function<void(Entity&)>>> commands;
	bool input_taken(sf::Keyboard::Key input1, sf::Keyboard::Key input2, sf::Keyboard::Key input3, sf::Keyboard::Key input4, sf::Keyboard::Key input5); // returns true if one is there
	bool inputs_taken(sf::Keyboard::Key input1, sf::Keyboard::Key input2, sf::Keyboard::Key input3, sf::Keyboard::Key input4, sf::Keyboard::Key input5); // returns true if all are there
};