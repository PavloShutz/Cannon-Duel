#include "Cannon.h"

Cannon::Cannon(sf::Vector2f position, std::string path)
    : lives(INITIAL_LIVES), shootCooldown(0.0f)
{
    cannonTexture.loadFromFile(path);
    cannonSprite.setTexture(cannonTexture);
    cannonSprite.setPosition(position);
}

void Cannon::resetLives() { lives = INITIAL_LIVES; }
