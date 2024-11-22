#include "Game.h"
#include <iostream>

Game::Game() :  m_window("Cannon Duel", sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT))
{
    m_gameRunning = true;
    bgTexture.loadFromFile("images/bg.png");
    bgSprite.setTexture(bgTexture);
	LoadFont();
}

Game::~Game() {}

void Game::HandleInput(float deltaTime) {
    // left player
    movePlayer(m_tiefighter, sf::Keyboard::W, sf::Keyboard::S, deltaTime);
    generateBullet(m_tiefighter, m_bullets1, sf::Keyboard::D, sf::Color(2, 250, 71), FIGHTER_SIZE.x);
    
    // right player
    movePlayer(m_starfighter, sf::Keyboard::Up, sf::Keyboard::Down, deltaTime);
    generateBullet(m_starfighter, m_bullets2, sf::Keyboard::Left, sf::Color(250, 2, 2), -BULLET_SIZE.x);
}

int Game::LoadFont() {
	if (!m_font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
		return -1;
	}
	return 0;
}

Window* Game::GetWindow() {
    return &m_window;
}

void Game::drawText(const sf::Font& font,
    const std::string& text, sf::Vector2f position,
    unsigned int size, sf::Color color)
{
    sf::Text displayText(text, font, size);
    displayText.setFillColor(color);
    displayText.setPosition(position);
    m_window.Draw(displayText);
}

void Game::Update() {
	m_window.Update();
    float deltaTime = m_clock.restart().asSeconds();

    if (m_gameRunning) {
        if (m_tiefighter.shootCooldown > 0) m_tiefighter.shootCooldown -= deltaTime;
        if (m_starfighter.shootCooldown > 0) m_starfighter.shootCooldown -= deltaTime;

        HandleInput(deltaTime);

        moveBullets(m_bullets1, 1, deltaTime);
        moveBullets(m_bullets2, -1, deltaTime);

        m_bullets1.erase(remove_if(m_bullets1.begin(), m_bullets1.end(),
            [](const sf::RectangleShape& bullet) { return bullet.getPosition().x > WINDOW_WIDTH; }),
            m_bullets1.end());
        m_bullets2.erase(remove_if(m_bullets2.begin(), m_bullets2.end(),
            [](const sf::RectangleShape& bullet) { return bullet.getPosition().x < 0; }),
            m_bullets2.end());

        checkHitFighter(m_bullets1, m_explosions, m_starfighter);
        checkHitFighter(m_bullets2, m_explosions, m_tiefighter);

        if (!m_explosions.empty()) m_explosions.erase(m_explosions.begin());

        if (m_tiefighter.lives <= 0 || m_starfighter.lives <= 0)
            m_gameRunning = false;
    }
    else
        checkRestart(m_tiefighter, m_starfighter);
}

void Game::Render() {
	m_window.BeginDraw();
    m_window.Draw(bgSprite);
    m_window.Draw(m_tiefighter.fighterSprite);
    m_window.Draw(m_starfighter.fighterSprite);

    for (auto& bullet : m_bullets1)
        m_window.Draw(bullet);
    for (auto& bullet : m_bullets2)
        m_window.Draw(bullet);
    for (auto& explosion : m_explosions)
        m_window.Draw(explosion);

    drawText(m_font, "Blue Lives: " + std::to_string(m_tiefighter.lives), {10, 10}, 20, sf::Color::Blue);
    drawText(m_font, "Red Lives: " + std::to_string(m_starfighter.lives), { 600, 10 }, 20, sf::Color::Red);

    if (!m_gameRunning) {
        std::string winner = (m_tiefighter.lives == 0 ? "Red" : "Blue");
        sf::Color winnerColor = (winner == "Red" ? sf::Color::Red : sf::Color::Blue);
        drawText(m_font, winner + " wins!", { 300, 100 }, 100, winnerColor);
        drawText(m_font, "Game Over!", { 300, 250 }, 50, sf::Color::White);
        drawText(m_font, "Press 'Space' to restart game", { 300, 350 }, 25, sf::Color::White);
    }
	m_window.EndDraw();
}

bool checkCollision(const sf::RectangleShape& bullet, const sf::Sprite& fighter) {
    return bullet.getGlobalBounds().intersects(fighter.getGlobalBounds());
}

void moveBullets(std::vector<sf::RectangleShape>& bullets, int direction, float deltaTime) {
    for (auto& bullet : bullets)
        bullet.move(direction * BULLET_SPEED * deltaTime, 0);
}

void checkHitFighter(std::vector<sf::RectangleShape>& bullets, std::vector<sf::CircleShape>& explosions, Fighter& fighter) {
    for (auto& bullet : bullets) {
        if (checkCollision(bullet, fighter.fighterSprite)) {
            fighter.lives--;
            explosions.push_back(sf::CircleShape(20, 20));
            explosions.back().setFillColor(sf::Color::Yellow);
            explosions.back().setPosition(fighter.fighterSprite.getPosition());
            if (fighter.fighterSprite.getPosition().x - (WINDOW_WIDTH / 2) > 0)
                bullet.setPosition(-WINDOW_WIDTH, -100);
            else
                bullet.setPosition(WINDOW_WIDTH, -100);
        }
    }
}

void movePlayer(Fighter& fighter, sf::Keyboard::Key up, sf::Keyboard::Key down, float deltaTime) {
    if (sf::Keyboard::isKeyPressed(up) && fighter.fighterSprite.getPosition().y > 0)
        fighter.fighterSprite.move(0, -FIGHTER_SPEED * deltaTime);
    if (sf::Keyboard::isKeyPressed(down) && fighter.fighterSprite.getPosition().y < WINDOW_HEIGHT - FIGHTER_SIZE.y)
        fighter.fighterSprite.move(0, FIGHTER_SPEED * deltaTime);
}

void generateBullet(Fighter& fighter, std::vector<sf::RectangleShape>& bullets,
    sf::Keyboard::Key shoot,  sf::Color color, float offset) {
    if (sf::Keyboard::isKeyPressed(shoot) && fighter.shootCooldown <= 0) {
        sf::RectangleShape bullet(BULLET_SIZE);
        bullet.setPosition(fighter.fighterSprite.getPosition().x + offset, fighter.fighterSprite.getPosition().y + FIGHTER_SIZE.y / 2 - BULLET_SIZE.y / 2);
        bullet.setFillColor(color);
        bullets.push_back(bullet);
        fighter.shootCooldown = SHOOT_COOLDOWN;
    }
}

void Game::checkRestart(Fighter& l_fighter1, Fighter& l_fighter2) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !m_gameRunning)
    {
        m_gameRunning = true;
        l_fighter1.resetLives();
        l_fighter2.resetLives();
        m_bullets1.clear();
        m_bullets2.clear();
    }
}