#include "Window2.h"

void Window::run(const sf::Event& event)
{
    if (!window.isOpen())
        return;
    if (event.type == sf::Event::Resized)
        resize();
    else if (event.type == sf::Event::Closed)
        window.close();
}

void Window::create(const std::string& name)
{
    window.create(sf::VideoMode(Options::window_size_x, Options::window_size_y), name);
    window.setFramerateLimit(Options::fps);
}

void Window::create(sf::Vector2i size, const std::string& name)
{
    window.create(sf::VideoMode(size.x, size.y), name);
    window.setFramerateLimit(Options::fps);
}

Window::Window(const std::string& name)
{
    create(name);
}

Window::Window(sf::Vector2i size, const std::string& name)
{
    create(size, name);
}

void Window::resize()
{
    sf::View old = window.getView();
    sf::View view;
    sf::Vector2f new_size = static_cast<sf::Vector2f>(window.getSize());
    view.setSize(new_size);
    sf::Vector2f dist = old.getCenter() + (view.getSize() - old.getSize()) / 2.f;
    view.setCenter(dist);
    window.setView(view);
    Options::window_size_x = new_size.x;
    Options::window_size_y = new_size.y;
}

void Window::move(sf::Vector2i direction)
{
    sf::View view = window.getView();
    sf::Vector2f viewPosition = view.getCenter();
    viewPosition += static_cast<sf::Vector2f>(direction);
    view.setCenter(viewPosition);
    window.setView(view);
}