#ifndef CANNON_H
#define CANNON_H
#include <SFML/Graphics.hpp>

constexpr float FIGHTER_SPEED = 400.0f;
constexpr float BULLET_SPEED = 1000.0f;
const sf::Vector2f BULLET_SIZE(20, 5);
const sf::Vector2f FIGHTER_SIZE(120, 100);
constexpr float SHOOT_COOLDOWN = 0.5f;
constexpr int INITIAL_LIVES = 3;

struct Fighter {
    sf::Texture fighterTexture;
    sf::Sprite fighterSprite;
    int lives;
    float shootCooldown;

    Fighter(sf::Vector2f position, std::string path);

    void resetLives();
};
#endif
