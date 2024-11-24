#include "Game.h"
#include <iostream>

Game::Game() :  m_window("Star Duel", sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT))
{
    m_gameRunning = true;
    
    LoadTextures();
    SetupSprites();

    LoadAudio();
    SetupAudio();

    LoadFonts();
}

Game::~Game() {}

void Game::LoadTextures()
{
    bgTexture.loadFromFile("images/bg.png");

    empireTexture.loadFromFile("images/empire.png");

    rebelsTexture.loadFromFile("images/rebels.png");
}

void Game::SetupSprites()
{
    bgSprite.setTexture(bgTexture);

    empireSprite.setTexture(empireTexture);
    empireSprite.setPosition(0.2 * WINDOW_WIDTH, 0.3 * WINDOW_HEIGHT);

    rebelsSprite.setTexture(rebelsTexture);
    rebelsSprite.setPosition(0.2 * WINDOW_WIDTH, 0.3 * WINDOW_HEIGHT);
}

void Game::LoadAudio()
{
    music.openFromFile("sound/music.mp3");
    explosionBuffer.loadFromFile("sound/explosion.mp3");
    empireBuffer.loadFromFile("sound/empire.mp3");
    rebelsBuffer.loadFromFile("sound/rebels.mp3");
}

void Game::SetupAudio()
{
    explosionSound.setBuffer(explosionBuffer);
    explosionSound.setVolume(40.f);
    empireSound.setBuffer(empireBuffer);
    rebelsSound.setBuffer(rebelsBuffer);
    music.setLoop(true);
    music.play();
}

void Game::HandleInput(float deltaTime) {
    // left player
    movePlayer(m_tiefighter, sf::Keyboard::W, sf::Keyboard::S, deltaTime);
    generateBullet(m_tiefighter, m_bullets1, sf::Keyboard::D, sf::Color(2, 250, 71), FIGHTER_SIZE.x);
    
    // right player
    movePlayer(m_starfighter, sf::Keyboard::Up, sf::Keyboard::Down, deltaTime);
    generateBullet(m_starfighter, m_bullets2, sf::Keyboard::Left, sf::Color(250, 2, 2), -BULLET_SIZE.x);
}

int Game::LoadFonts() {
#ifdef _WIN32
    if (!m_winFont.loadFromFile("fonts/starjout/Starjout.ttf")) {
        return -1;
    }
#elif _MAC
    if (!m_winFont.loadFromFile("fonts/__MACOSX/starjout/._Starjout.ttf")) {
        return -1;
    }
#endif
	if (!m_livesFont.loadFromFile("fonts/anakinmono.ttf")) {
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

        checkHitFighter(m_bullets1, m_starfighter);
        checkHitFighter(m_bullets2, m_tiefighter);

        if (m_tiefighter.lives <= 0 || m_starfighter.lives <= 0)
        {
            music.stop();
            m_tiefighter.lives <= 0 ? m_tiefighter.fighterTexture.loadFromFile("images/explosion.png") :
                m_starfighter.fighterTexture.loadFromFile("images/explosion.png");
            explosionSound.play();
            if (m_tiefighter.lives <= 0)
                rebelsSound.play();
            else
                empireSound.play();
            m_gameRunning = false;
        }
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

    drawText(m_livesFont, "Lives: " + std::to_string(m_tiefighter.lives), {10, 10}, 30, sf::Color(2, 250, 71));
    drawText(m_livesFont, "Lives: " + std::to_string(m_starfighter.lives), { WINDOW_WIDTH - 200, 10 }, 30, sf::Color(250, 2, 2));

    if (!m_gameRunning) {
        std::string winner = (m_tiefighter.lives == 0 ? "Rebellion" : "Empire");
        sf::Color winnerColor = (winner == "Rebellion" ? sf::Color(250, 2, 2) : sf::Color(158, 60, 60));
        m_window.Draw((winner == "Rebellion" ? rebelsSprite : empireSprite));
        drawText(m_winFont, winner + " wins!",
            { 0.2 * WINDOW_WIDTH + 300, 0.3 * WINDOW_HEIGHT + 40 }, 120, winnerColor
        );
        drawText(m_livesFont, "Press 'Space' to restart game", { 0.2 * WINDOW_WIDTH + 100, 0.3 * WINDOW_HEIGHT + 300 }, 50, sf::Color::Black);
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

void checkHitFighter(std::vector<sf::RectangleShape>& bullets, Fighter& fighter) {
    for (auto& bullet : bullets) {
        if (checkCollision(bullet, fighter.fighterSprite)) {
            fighter.lives--;
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
        l_fighter1.fighterTexture.loadFromFile("images/tiefighter.png");
        l_fighter2.fighterTexture.loadFromFile("images/starfighter.png");
        m_bullets1.clear();
        m_bullets2.clear();
        music.play();
    }
}