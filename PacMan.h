 #pragma once
#include "Entity.h"
#include "Map_Structures.h"

class PacMan :
    public Entity
{
public:
    PacMan(sf::Color color, const sf::Vector2f start_pos, std::chrono::high_resolution_clock::time_point game_start);
    virtual void draw(sf::RenderWindow& window) override;
    void eat(Object object);
private:
    virtual void animate(std::chrono::milliseconds time_passed) override;
    virtual void colision(Entity& other) override;
    virtual void rotate(Direction direction) override;
    sf::ConvexShape mouth;
    static uint16_t mouth_points;
    static sf::RenderStates render_states;
    float mouth_angle = 0;
    bool switch_animation = true;
    bool enpowered = false;
    Direction direction = RIGHT;
    uint16_t points = 0;
    std::chrono::high_resolution_clock::time_point time_enpowered;
};

