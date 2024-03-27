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

void BodyPart::draw(sf::RenderWindow& window, sf::Vector2f pos, sf::Color color, int rotation)
{
	shape.setFillColor(color);
	shape.setPosition(pos + local_pos);
    shape.setRotation(rotation);
	window.draw(shape);
}

void BodyPart::resize(float scale)
{
    local_pos *= scale;
}
