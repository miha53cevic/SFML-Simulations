#include "window.h"

Window::Window() { Setup(sf::Vector2u(640, 480), "Default window"); }
Window::Window(sf::Vector2u l_size, std::string l_name) { Setup(l_size, l_name); }

Window::~Window() { Destroy(); }

void Window::Destroy() { m_window.close(); }

void Window::Setup(sf::Vector2u l_size, std::string l_name)
{
	m_window.create(sf::VideoMode(l_size.x, l_size.y), l_name);
	m_done = false;
	m_window.setVerticalSyncEnabled(true);

	m_windowSize = l_size;
}

void Window::Update()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			m_done = true;
	}
}

bool Window::isDone() { return m_done; }

void Window::Draw(sf::Drawable& l_drawable) { m_window.draw(l_drawable); }

void Window::BeginDraw(sf::Color color) { m_window.clear(color); }
void Window::EndDraw() { m_window.display(); }

sf::Vector2u Window::getSize() { return m_windowSize; }
sf::RenderWindow* Window::getWindow() { return &m_window; }