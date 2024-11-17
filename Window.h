#ifndef WINDOW_H
#define WINDOW_H
#include <SFML/Graphics.hpp>

class Window {
public:
	Window();
	Window(const std::string& l_title, const sf::Vector2u& l_size);
	~Window();

	void Update();

	void Draw(sf::Drawable& l_drawable);

	void BeginDraw(); // Clear the window.
	void EndDraw(); // Display the contents.

	bool IsDone() const;
	bool IsFullscreen() const;

	void ToggleFullscreen();

private:
	void Setup(const std::string& l_title, const sf::Vector2u& l_size);
	void Destroy();
	void Create();

	sf::RenderWindow m_window;
	std::string m_windowTitle;
	sf::Vector2u m_windowSize;
	bool m_isDone;
	bool m_isFullscreen;
};
#endif
