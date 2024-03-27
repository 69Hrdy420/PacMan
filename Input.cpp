#include "Input.h"


static void Left(Entity& entity) 
{
    entity.change_directions(LEFT);
}

static void Right(Entity& entity) 
{
    entity.change_directions(RIGHT);
}

static void Up(Entity& entity)
{
    entity.change_directions(UP);
}

static void Down(Entity& entity)
{
    entity.change_directions(DOWN);
}

static void Stop(Entity& entity) 
{
    entity.change_directions(IDLE);
}

bool Input::add(Entity& entity, sf::Keyboard::Key left, sf::Keyboard::Key right, sf::Keyboard::Key up, sf::Keyboard::Key down, sf::Keyboard::Key stop)
{
    if (input_taken(left, right, up, down, stop))
        return false;
    commands[left] = std::make_pair(&entity, Left);
    commands[right] = std::make_pair(&entity, Right);
    commands[up] = std::make_pair(&entity, Up);
    commands[down] = std::make_pair(&entity, Down);
    commands[stop] = std::make_pair(&entity, Stop);
    return true;
}

void Input::run(sf::Keyboard::Key key)
{
    auto it = commands.find(key);
    if (it != commands.end())
    {
        it->second.second(*it->second.first);
    }
}

void Input::remove(sf::Keyboard::Key left, sf::Keyboard::Key right, sf::Keyboard::Key up, sf::Keyboard::Key down, sf::Keyboard::Key stop)
{
    commands.erase(left);
    commands.erase(right);
    commands.erase(up);
    commands.erase(down);
    commands.erase(stop);
}

void Input::remove(const Entity& entity)
{
    for (auto it = commands.begin(); it != commands.end(); )
    {
        if (it->second.first == &entity)
        {
            it = commands.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

bool Input::get_input1(Entity& entity)
{
    if (input_taken(sf::Keyboard::A, sf::Keyboard::W, sf::Keyboard::D, sf::Keyboard::S, sf::Keyboard::Space))
        return false;
    commands[sf::Keyboard::A] = std::make_pair(&entity, Left);
    commands[sf::Keyboard::D] = std::make_pair(&entity, Right);
    commands[sf::Keyboard::W] = std::make_pair(&entity, Up);
    commands[sf::Keyboard::S] = std::make_pair(&entity, Down);
    commands[sf::Keyboard::Space] = std::make_pair(&entity, Stop);
    return true;
}

bool Input::get_input2(Entity& entity)
{
    if (input_taken(sf::Keyboard::Right, sf::Keyboard::Left, sf::Keyboard::Down, sf::Keyboard::Up, sf::Keyboard::RShift))
        return false;
    commands[sf::Keyboard::Left] = std::make_pair(&entity, Left);
    commands[sf::Keyboard::Right] = std::make_pair(&entity, Right);
    commands[sf::Keyboard::Up] = std::make_pair(&entity, Up);
    commands[sf::Keyboard::Down] = std::make_pair(&entity, Down);
    commands[sf::Keyboard::RShift] = std::make_pair(&entity, Stop);
    return true;
}

bool Input::input_taken(sf::Keyboard::Key input1, sf::Keyboard::Key input2, sf::Keyboard::Key input3, sf::Keyboard::Key input4, sf::Keyboard::Key input5)
{
    return commands.count(input1) || commands.count(input2) || commands.count(input3) || commands.count(input4) || commands.count(input5);
}

bool Input::inputs_taken(sf::Keyboard::Key input1, sf::Keyboard::Key input2, sf::Keyboard::Key input3, sf::Keyboard::Key input4, sf::Keyboard::Key input5)
{
    return commands.count(input1) && commands.count(input2) && commands.count(input3) && commands.count(input4) && commands.count(input5);
}

void Input::remove_input1()
{
    if (inputs_taken(sf::Keyboard::A, sf::Keyboard::W, sf::Keyboard::D, sf::Keyboard::S, sf::Keyboard::Space))
        remove(sf::Keyboard::A, sf::Keyboard::W, sf::Keyboard::D, sf::Keyboard::S, sf::Keyboard::Space);
}

void Input::remove_input2()
{
    if (inputs_taken(sf::Keyboard::Right, sf::Keyboard::Left, sf::Keyboard::Down, sf::Keyboard::Up, sf::Keyboard::RShift))
        remove(sf::Keyboard::Right, sf::Keyboard::Left, sf::Keyboard::Down, sf::Keyboard::Up, sf::Keyboard::RShift);
}
