#include "Window.h"

Window::Window() { Setup("Sandbox", { 640, 480 }); }

Window::Window(const std::string& l_title, const sf::Vector2u& l_size) {
	Setup(l_title, l_size);
}

Window::~Window() { Destroy(); }

void Window::Setup(const std::string& l_title, const sf::Vector2u& l_size) {
	m_windowTitle = l_title;
	m_windowSize = l_size;
	m_isDone = false;
	m_isFullscreen = false;
	Create();
}

void Window::Create() {
	auto style = (m_isFullscreen ? sf::Style::Fullscreen : sf::Style::Default);
	m_window.create({ m_windowSize.x, m_windowSize.y }, m_windowTitle, style);
	m_window.setFramerateLimit(60);
}

void Window::Destroy() { m_window.close(); }

void Window::Update() {
	sf::Event event;
	while (m_window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			m_isDone = true;
		}
		else if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::F5) {
				ToggleFullscreen();
			}
			if (event.key.code == sf::Keyboard::Escape) {
				m_isDone = true;
			}
		}
	}
}

void Window::ToggleFullscreen() {
	m_isFullscreen = !m_isFullscreen;
	Destroy();
	Create();
}

bool Window::IsDone() const { return m_isDone; }
bool Window::IsFullscreen() const { return m_isFullscreen; }

void Window::BeginDraw() {
	m_window.clear();
}

void Window::EndDraw() {
	m_window.display();
}

void Window::Draw(sf::Drawable& l_drawable) {
	m_window.draw(l_drawable);
}

sf::Vector2u Window::GetSize() const {
	return { m_window.getSize() };
}