#ifndef GAME_H
#define GAME_H
#include "Window.h"
#include "Fighter.h"
#include <vector>
#include <sstream>
#include <SFML/Audio.hpp>

constexpr int WINDOW_WIDTH  = 1920;
constexpr int WINDOW_HEIGHT = 1080;

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

	void checkRestart(Fighter& l_cannon1, Fighter& l_cannon2);

private:
	int LoadFont();

	Window m_window;
	sf::Font m_livesFont;
	sf::Font m_winFont;
	
	sf::Texture bgTexture;
	sf::Sprite bgSprite;

	sf::Texture empireTexture;
	sf::Sprite empireSprite;

	sf::Texture rebelsTexture;
	sf::Sprite rebelsSprite;

	sf::Music music;

	Fighter m_tiefighter{ 
		{ 50, (WINDOW_HEIGHT / 2) - FIGHTER_SIZE.y / 2 },
		"images/tiefighter.png"
	};
	Fighter m_starfighter{
		{ (WINDOW_WIDTH - 50) - FIGHTER_SIZE.x, (WINDOW_HEIGHT / 2) - FIGHTER_SIZE.y / 2},
		"images/starfighter.png"
	};

	std::vector<sf::RectangleShape> m_bullets1;
	std::vector<sf::RectangleShape> m_bullets2;

	sf::Clock m_clock;

	bool m_gameRunning;
};

bool checkCollision(const sf::RectangleShape& bullet, const sf::Sprite& fighter);

void moveBullets(std::vector<sf::RectangleShape>& bullets, int direction, float deltaTime);

void checkHitFighter(std::vector<sf::RectangleShape>& bullets, Fighter& fighter);

void movePlayer(Fighter& fighter, sf::Keyboard::Key up, sf::Keyboard::Key down, float deltaTime);

void generateBullet(Fighter& fighter, std::vector<sf::RectangleShape>& bullets,
	sf::Keyboard::Key shoot, sf::Color color, float offset);
#endif
