#include "Fighter.h"

Fighter::Fighter(sf::Vector2f position, std::string path)
    : lives(INITIAL_LIVES), shootCooldown(0.0f)
{
    fighterTexture.loadFromFile(path);
    fighterSprite.setTexture(fighterTexture);
    fighterSprite.setPosition(position);
}

void Fighter::resetLives() { lives = INITIAL_LIVES; }
