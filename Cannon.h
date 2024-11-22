#ifndef CANNON_H
#define CANNON_H
#include <SFML/Graphics.hpp>

constexpr float CANNON_SPEED = 200.0f;
constexpr float BULLET_SPEED = 400.0f;
const sf::Vector2f BULLET_SIZE(10, 5);
const sf::Vector2f CANNON_SIZE(20, 60);
constexpr float SHOOT_COOLDOWN = 0.5f;
constexpr int INITIAL_LIVES = 3;

struct Cannon {
    sf::Texture cannonTexture;
    sf::Sprite cannonSprite;
    int lives;
    float shootCooldown;

    Cannon(sf::Vector2f position, std::string path);

    void resetLives();
};
#endif
