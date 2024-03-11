#include "PacMan.h"

static const float PI = 3.141592653589;
static const uint8_t mouth_width_fraction = 4;

uint16_t PacMan::mouth_points;
sf::RenderStates PacMan::render_states = sf::BlendMode(sf::BlendMode::Zero, sf::BlendMode::OneMinusDstColor);

PacMan::PacMan(sf::Color color, std::chrono::high_resolution_clock::time_point game_start)
    :Entity(color, game_start)
{
	mouth.setFillColor(sf::Color::Red);
	body = { BodyPart(Shapes::get(PACMAN), { 0,0 }) };

	mouth_points = Options::num_circle_points / 4 + 1;
	if (mouth_points < 4)
		mouth_points = 4;
	else if (mouth_points % 2 == 1)
		mouth_points--;

	mouth.setPointCount(mouth_points);
	mouth.setPoint(0, { 0, 0 });
	for (int index = 1; index < mouth_points; index++)
		mouth.setPoint(index, { Options::object_size, 0 });
}
void PacMan::animate(std::chrono::milliseconds time_passed)
{
	float distance_ratio = static_cast<float>(time_passed.count()) / static_cast<float>(Options::animation_speed_miliseconds);
	float x, y;
	if (switch_animation)
	{
		mouth_angle += PI * distance_ratio / mouth_width_fraction;
		if (mouth_angle > PI / mouth_width_fraction)
			switch_animation = false;
	}
	else
	{
		mouth_angle -= (PI * distance_ratio / mouth_width_fraction);
		if (mouth_angle < 0)
			switch_animation = true;
	}
	int bot = 1;
	int top = mouth_points - 1;
	int num = mouth_points;
	for (;;)
	{
		float angle = mouth_angle * num / mouth_points;
		x = Options::object_size * std::cos(angle);
		y = Options::object_size * std::sin(angle);
		mouth.setPoint(top, { x, y });
		mouth.setPoint(bot, { x, -y });

		bot++;
		top--;
		num -= 2;
		if (bot == top)
			break;
	}
	mouth.setPosition(pos);
}
void PacMan::draw(sf::RenderWindow& window)
{
	auto current_time = std::chrono::high_resolution_clock::now();
	auto time_passed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_animation);
	last_animation = current_time;
	animate(time_passed);

	body[0].draw(window, pos, color);
	window.draw(mouth, render_states);
}