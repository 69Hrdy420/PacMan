#pragma once
#include "Entity.h"

class Ghost :
    public Entity
{
public:
    Ghost(sf::Color color, const sf::Vector2f start_pos, std::chrono::high_resolution_clock::time_point game_start);
    virtual void resize(float scale) override;
private:
    virtual void animate(std::chrono::milliseconds time_passed) override;
    std::vector<BodyPart> create_body(float scale);
    virtual void rotate(Direction direction) override;
    bool switch_animation = true;
};

