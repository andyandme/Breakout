#pragma once

#include "PowerupBase.h"

class PowerupSlowBall : public PowerupBase
{
public:
    PowerupSlowBall(sf::RenderWindow* window, Paddle* paddle, Ball* ball);
    ~PowerupSlowBall();

    std::pair<POWERUPS, float> applyEffect() override; // Method to apply the power-up effect
};
