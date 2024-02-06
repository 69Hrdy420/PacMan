#pragma once
#include "Entity.h"

static enum Direction
{
    RIGHT, LEFT, UP, DOWN
};

class PacMan :
    public Entity
{
public:
    PacMan(sf::Color color, std::chrono::high_resolution_clock::time_point game_start);
    virtual void draw() override;
private:
    virtual void animate(std::chrono::milliseconds time_passed) override;
    sf::ConvexShape mouth;
    static uint16_t mouth_points;
    static sf::RenderStates render_states;
    float mouth_angle = 0;
    bool switch_animation = true;
    Direction direction = RIGHT;
};

