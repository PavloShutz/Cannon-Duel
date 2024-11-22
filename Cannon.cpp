#include "Cannon.h"

Cannon::Cannon(sf::Vector2f position, sf::Color color, std::string path)
    : lives(INITIAL_LIVES), shootCooldown(0.0f)
{
    shape.setSize(CANNON_SIZE);
    shape.setPosition(position);
    shape.setFillColor(color);
    cannonTexture.loadFromFile(path);
    cannonSprite.setTexture(cannonTexture);
    cannonSprite.setPosition(position);
}

void Cannon::resetLives() { lives = INITIAL_LIVES; }
