#include "Application.h"

Application::Application() : m_FallingText(m_window.getSize()) {}
Application::Application(sf::Vector2u l_size, std::string l_name) : m_window(l_size, l_name), m_FallingText(m_window.getSize()) {}
Application::~Application() {}

void Application::HandleInput()
{
	//input
}

void Application::Update()
{
	m_window.Update();
	m_FallingText.Update();
}

void Application::Render()
{
	m_window.BeginDraw(sf::Color::Black);

	m_FallingText.Draw(*m_window.getWindow());

	m_window.EndDraw();
}

void Application::RestartClock()
{
	m_elapsed = m_clock.restart();
}

Window* Application::getWindow() { return &m_window; }

//For MatrixRain Only
FallingText* Application::getMatrixRain() { return &m_FallingText; }