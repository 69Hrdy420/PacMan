#include "Options.h"

bool Options::read()
{
	return true;
}
void Options::save()
{
	return;
}

float Options::object_size = 150;
uint16_t Options::window_size_x = 1200;
uint16_t Options::window_size_y = 1000;
uint16_t Options::num_circle_points = 150;
uint16_t Options::animation_speed_miliseconds = 300;
uint16_t Options::fps = 120;