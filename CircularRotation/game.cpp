#include "game.h"

Game::Game() : m_hexagongame(m_window.getSize())
{
	//Load Default settings && Textures
}
Game::Game(const sf::Vector2u& l_size, const std::string& l_name) : m_window(l_size, l_name), m_hexagongame(m_window.getSize()) {}
Game::~Game() {}

Window* Game::getWindow()
{
	return &m_window;
}

void Game::HandleInput()
{
	//input
	m_hexagongame.Input();
}

void Game::Update()
{
	//Event System
	m_window.Update();
	m_hexagongame.Update(m_elapsed);
}

void Game::Render()
{
	m_window.BeginDraw(sf::Color::Color(128, 128, 128, 255));
	
	m_hexagongame.Draw(*m_window.getWindow());

	m_window.EndDraw();
}

void Game::RestartClock()
{
	m_elapsed = m_clock.restart();
}

HexagonGame* Game::getHexagonGame() { return &m_hexagongame; }