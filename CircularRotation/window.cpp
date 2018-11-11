#include "window.h"

Window::Window() { Setup(sf::Vector2u(640, 480), "Default Window"); }
Window::Window(const sf::Vector2u& l_size, const std::string& l_name) { Setup(l_size, l_name); }
Window::~Window() { Destroy(); }

void Window::Setup(const sf::Vector2u& l_size, const std::string& l_name)
{
	m_window.create(sf::VideoMode(l_size.x, l_size.y), l_name);
	bIsDone = false;
	m_window.setVerticalSyncEnabled(true);
}

void Window::Destroy()
{
	m_window.close();
}

bool Window::IsDone()
{
	return bIsDone;
}

void Window::BeginDraw(sf::Color l_color)
{
	m_window.clear(l_color);
}

void Window::EndDraw()
{
	m_window.display();
}

void Window::Draw(sf::Drawable& l_drawable)
{
	m_window.draw(l_drawable);
}

void Window::Update()
{
	sf::Event event;

	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			bIsDone = true;
		}
	}
}

sf::Vector2u Window::getSize()
{
	return m_window.getSize();
}

sf::RenderWindow* Window::getWindow()
{
	return &m_window;
}