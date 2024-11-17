#ifndef GAME_H
#define GAME_H
#include "Window.h"
#include "Cannon.h"
#include <vector>
#include <sstream>

class Game {
public:
	Game();
	~Game();

	void HandleInput(float deltaTime);
	void Update();
	void Render();
	Window* GetWindow();

	void drawText(const sf::Font& font,
		const std::string& text, sf::Vector2f position,
		unsigned int size, sf::Color color);

private:
	int LoadFont();

	Window m_window;
	sf::Font m_font;
	
	Cannon m_cannon1{ { 50, 300 - CANNON_SIZE.y / 2 }, sf::Color::Blue };
	Cannon m_cannon2{ {750 - CANNON_SIZE.x, 300 - CANNON_SIZE.y / 2}, sf::Color::Red };

	std::vector<sf::RectangleShape> m_bullets1;
	std::vector<sf::RectangleShape> m_bullets2;

	std::vector<sf::CircleShape> m_explosions;

	sf::Clock m_clock;

	bool m_gameRunning;
};

bool checkCollision(const sf::RectangleShape& a, const sf::RectangleShape& b);

//void drawText(sf::RenderWindow& window, const sf::Font& font,
//	const std::string& text, sf::Vector2f position,
//	unsigned int size, sf::Color color);

void moveBullets(std::vector<sf::RectangleShape>& bullets, int direction, float deltaTime);

void checkHitCannon(std::vector<sf::RectangleShape>& bullets, std::vector<sf::CircleShape>& explosions, Cannon& cannon);

void movePlayer(Cannon& cannon, sf::Keyboard::Key up, sf::Keyboard::Key down, float deltaTime);

void generateBullet(Cannon& cannon, std::vector<sf::RectangleShape>& bullets,
	sf::Keyboard::Key shoot, sf::Color color, float offset);
#endif
