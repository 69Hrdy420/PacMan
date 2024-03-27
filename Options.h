#include <cstdint>
#include <string>
#include <vector>

namespace Options
{
	void read();
	void save();

    extern float object_size;
    extern uint16_t window_size_x;
    extern uint16_t window_size_y;
    extern uint16_t num_circle_points;
    extern uint16_t animation_speed_miliseconds;
    extern float game_speed;
    extern uint16_t fps;
    extern float tolerance;
    extern float hitbox;

    extern std::string default_map;
    extern std::string default_map2;
    std::vector<int> get_default_map();
}

