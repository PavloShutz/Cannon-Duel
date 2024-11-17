#include "Game.h"

Game::Game() : m_window("Cannon Duel", sf::Vector2u(800, 600))
{
    m_gameRunning = true;
	LoadFont();
}

Game::~Game() {}

void Game::HandleInput(float deltaTime) {
    // left player
    movePlayer(m_cannon1, sf::Keyboard::W, sf::Keyboard::S, deltaTime);
    generateBullet(m_cannon1, m_bullets1, sf::Keyboard::D, sf::Color::Blue, CANNON_SIZE.x);
    
    // right player
    movePlayer(m_cannon2, sf::Keyboard::Up, sf::Keyboard::Down, deltaTime);
    generateBullet(m_cannon2, m_bullets2, sf::Keyboard::Left, sf::Color::Red, -BULLET_SIZE.x);
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
        if (m_cannon1.shootCooldown > 0) m_cannon1.shootCooldown -= deltaTime;
        if (m_cannon2.shootCooldown > 0) m_cannon2.shootCooldown -= deltaTime;

        HandleInput(deltaTime);

        moveBullets(m_bullets1, 1, deltaTime);
        moveBullets(m_bullets2, -1, deltaTime);

        m_bullets1.erase(remove_if(m_bullets1.begin(), m_bullets1.end(),
            [](const sf::RectangleShape& bullet) { return bullet.getPosition().x > 800; }),
            m_bullets1.end());
        m_bullets2.erase(remove_if(m_bullets2.begin(), m_bullets2.end(),
            [](const sf::RectangleShape& bullet) { return bullet.getPosition().x < 0; }),
            m_bullets2.end());

        checkHitCannon(m_bullets1, m_explosions, m_cannon2);
        checkHitCannon(m_bullets2, m_explosions, m_cannon1);

        if (!m_explosions.empty()) m_explosions.erase(m_explosions.begin());

        if (m_cannon1.lives <= 0 || m_cannon2.lives <= 0)
            m_gameRunning = false;
    }
}

void Game::Render() {
	m_window.BeginDraw();
    m_window.Draw(m_cannon1.shape);
    m_window.Draw(m_cannon2.shape);

    for (auto& bullet : m_bullets1)
        m_window.Draw(bullet);
    for (auto& bullet : m_bullets2)
        m_window.Draw(bullet);
    for (auto& explosion : m_explosions)
        m_window.Draw(explosion);

    drawText(m_font, "Blue Lives: " + std::to_string(m_cannon1.lives), {10, 10}, 20, sf::Color::Blue);
    drawText(m_font, "Red Lives: " + std::to_string(m_cannon2.lives), { 600, 10 }, 20, sf::Color::Red);

    if (!m_gameRunning) {
        drawText(m_font, "Game Over!", { 300, 250 }, 50, sf::Color::White);
    }
	m_window.EndDraw();
}

bool checkCollision(const sf::RectangleShape& a, const sf::RectangleShape& b) {
    return a.getGlobalBounds().intersects(b.getGlobalBounds());
}

void moveBullets(std::vector<sf::RectangleShape>& bullets, int direction, float deltaTime) {
    for (auto& bullet : bullets)
        bullet.move(direction * BULLET_SPEED * deltaTime, 0);
}

void checkHitCannon(std::vector<sf::RectangleShape>& bullets, std::vector<sf::CircleShape>& explosions, Cannon& cannon) {
    for (auto& bullet : bullets) {
        if (checkCollision(bullet, cannon.shape)) {
            cannon.lives--;
            explosions.push_back(sf::CircleShape(20, 20));
            explosions.back().setFillColor(sf::Color::Yellow);
            explosions.back().setPosition(cannon.shape.getPosition());
            if (cannon.shape.getPosition().x - 400 > 0)
                bullet.setPosition(-100, -100);
            else
                bullet.setPosition(1000, 1000);
        }
    }
}

void movePlayer(Cannon& cannon, sf::Keyboard::Key up, sf::Keyboard::Key down, float deltaTime) {
    if (sf::Keyboard::isKeyPressed(up) && cannon.shape.getPosition().y > 0)
        cannon.shape.move(0, -CANNON_SPEED * deltaTime);
    if (sf::Keyboard::isKeyPressed(down) && cannon.shape.getPosition().y < 600 - CANNON_SIZE.y)
        cannon.shape.move(0, CANNON_SPEED * deltaTime);
}

void generateBullet(Cannon& cannon, std::vector<sf::RectangleShape>& bullets,
    sf::Keyboard::Key shoot,  sf::Color color, float offset) {
    if (sf::Keyboard::isKeyPressed(shoot) && cannon.shootCooldown <= 0) {
        sf::RectangleShape bullet(BULLET_SIZE);
        bullet.setPosition(cannon.shape.getPosition().x + offset, cannon.shape.getPosition().y + CANNON_SIZE.y / 2 - BULLET_SIZE.y / 2);
        bullet.setFillColor(color);
        bullets.push_back(bullet);
        cannon.shootCooldown = SHOOT_COOLDOWN;
    }
}