#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>

class Window
{
public:
	Window();
	Window(sf::Vector2u l_size, std::string l_name);
	~Window();

	void Destroy();
	
	void BeginDraw(sf::Color color);
	void EndDraw();

	void Draw(sf::Drawable& l_drawable);

	void Setup(sf::Vector2u l_size, std::string l_name);
	void Update();

	bool isDone();

	sf::Vector2u getSize();
	sf::RenderWindow* getWindow();

private:
	sf::RenderWindow m_window;
	sf::Vector2u m_windowSize;

	bool m_done;
};