#include "BodyPart.h"

BodyPart::BodyPart(sf::ConvexShape& shape, sf::Vector2f local_pos)
	:shape(shape), local_pos(local_pos) {}
BodyPart::BodyPart(const BodyPart& other)
	: shape(other.shape), local_pos(other.local_pos) {}

BodyPart& BodyPart::operator=(const BodyPart& other)
{
    if (this != &other)
    {
        shape = other.shape;
        local_pos = other.local_pos;
    }
    return *this;
}

void BodyPart::draw(sf::Vector2f pos, sf::Color color)
{
	shape.setFillColor(color);
	shape.setPosition(pos + local_pos);
	window.draw(shape);
}