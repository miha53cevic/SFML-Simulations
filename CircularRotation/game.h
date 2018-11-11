#pragma once
#include "window.h"
#include "hexagon.h"

class Game
{
public:
	Game();
	Game(const sf::Vector2u& l_size, const std::string& l_name);
	~Game();

	void HandleInput();
	void Update();
	void Render();

	Window* getWindow();
	HexagonGame* getHexagonGame();

	void RestartClock();

private:
	Window m_window;

	HexagonGame m_hexagongame;

	sf::Clock m_clock;
	sf::Time m_elapsed;
};