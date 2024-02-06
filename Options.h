#include <cstdint>

namespace Options
{
	bool read();
	void save();

    extern float object_size;
    extern uint16_t window_size_x;
    extern uint16_t window_size_y;
    extern uint16_t num_circle_points;
    extern uint16_t animation_speed_miliseconds;
    extern uint16_t fps;
}

