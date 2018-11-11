#pragma once
#include "window.h"
#include "fallingtext.h"

class Application
{
public:
	Application();
	Application(sf::Vector2u l_size, std::string l_name);
	~Application();

	void HandleInput();
	void Update();
	void Render();
	void RestartClock();

	Window* getWindow();

	////For MatrixRain Only
	FallingText* getMatrixRain();

private:
	Window m_window;

	sf::Clock m_clock;
	sf::Time m_elapsed;

	FallingText m_FallingText;
};
