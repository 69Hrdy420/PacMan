#include "Shapes.h"

static const float PI = 3.141592653589;

std::vector<sf::ConvexShape> Shapes::shapes = std::vector<sf::ConvexShape>(4);
Shapes Shapes::instance;
float Shapes::scale = 1;

sf::ConvexShape& Shapes::get(int name)
{
	return shapes[name];
}

Shapes::Shapes()
{
	create_ghost_upperbody();
	create_ghost_legs();
    create_pacman();
    resize(0.4);
}

void Shapes::create_ghost_upperbody()
{
    uint16_t head_points = Options::num_circle_points / 2;
    float size = Options::object_size;
    if (head_points < 3)
        head_points = 3;
    sf::ConvexShape& upperbody = shapes[GHOST_BODY];
    upperbody.setPointCount(head_points + 2);
    for (int index = 0; index < head_points; index++)
    {
        float angle = PI * index / (head_points - 1);
        float x = size * std::cos(angle);
        float y = size * std::sin(angle);
        upperbody.setPoint(index + 1, sf::Vector2f(x, -y));
    }
    upperbody.setPoint(0, { size, size });
    upperbody.setPoint(head_points + 1, { -size, size });
}

void Shapes::create_ghost_legs()
{
    uint16_t num_points = Options::num_circle_points / 4;       
    float diameter = Options::object_size / 3;
    if (num_points < 2)
        num_points = 2;
    sf::ConvexShape& quater_circle1 = shapes[GHOST_LEGS1];
    sf::ConvexShape& quater_circle2 = shapes[GHOST_LEGS2];
    quater_circle1.setPointCount(num_points + 1);
    quater_circle2.setPointCount(num_points + 1);
    for (int index = 0; index < num_points; index++)
    {
        float angle = PI * index / (num_points - 1) / 2;
        float x = diameter * std::cos(angle);
        float y = diameter * std::sin(angle);
        quater_circle1.setPoint(index + 1, sf::Vector2f(x, y));
        quater_circle2.setPoint(index + 1, sf::Vector2f(-x, y));
    }
    quater_circle1.setPoint(0, { 0, 0 });
    quater_circle2.setPoint(0, { 0, 0 });
}

void Shapes::create_pacman()
{
    sf::ConvexShape& circle = shapes[PACMAN_BODY];
    circle.setPointCount(Options::num_circle_points + 1);
    for (int index = 1; index <= Options::num_circle_points; index++)
    {
        float angle = 2 * PI * (index - 1) / (Options::num_circle_points);
        float x = Options::object_size * std::cos(angle);
        float y = Options::object_size * std::sin(angle);
        circle.setPoint(index, { x, y });
    }
    circle.setPoint(0, circle.getPoint(Options::num_circle_points));
}

void Shapes::resize(float scale)
{
    for (sf::ConvexShape& shape : shapes)
    {
        sf::Vector2f origin = shape.getOrigin();
        for (int i = 0; i < shape.getPointCount(); i++)
            shape.setPoint(i, origin - (origin - shape.getPoint(i)) * scale);
    }
    Shapes::scale *= scale;
}

float Shapes::get_scale()
{
    return scale;
}