#include "Cannon.h"

Cannon::Cannon(sf::Vector2f position, sf::Color color)
    : lives(INITIAL_LIVES), shootCooldown(0.0f)
{
    shape.setSize(CANNON_SIZE);
    shape.setPosition(position);
    shape.setFillColor(color);
}