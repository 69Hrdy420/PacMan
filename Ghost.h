#pragma once
#include "Entity.h"

class Ghost :
    public Entity
{
public:
    Ghost(sf::Color color, std::chrono::high_resolution_clock::time_point game_start);
private:
    virtual void animate(std::chrono::milliseconds time_passed) override;
    std::vector<BodyPart> create_body();
    bool switch_animation = true;
};

