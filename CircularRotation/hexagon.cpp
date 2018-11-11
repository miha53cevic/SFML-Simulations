#include "hexagon.h"

HexagonGame::HexagonGame(const sf::Vector2u& l_windowSize) { Setup(l_windowSize); }
HexagonGame::~HexagonGame() {}

void HexagonGame::Update(sf::Time elapsed)
{
	if (m_degrees >= 360)
	{
		m_degrees = 0;
	}

	if (m_bLeftPressed)
	{
		m_bLeftPressed = false;

		m_degrees -= m_fSpeed * elapsed.asSeconds();
		m_player.rotate(-m_fSpeed * elapsed.asSeconds());
	}

	if (m_bRightPressed)
	{
		m_bRightPressed = false;

		m_degrees += m_fSpeed * elapsed.asSeconds();
		m_player.rotate(+m_fSpeed * elapsed.asSeconds());
	}
	
	m_player.setPosition({m_radiusOfPath * cos(toRad()), m_radiusOfPath * sin(toRad())});
}

void HexagonGame::Draw(sf::RenderWindow& window)
{
	window.draw(middle, kordinatni_sustav);
	window.draw(m_player, kordinatni_sustav);
}

void HexagonGame::Input()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		m_bLeftPressed = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		m_bRightPressed = true;
}

float HexagonGame::toRad()
{
	return m_degrees * (float)M_PI / 180;
}

void HexagonGame::CustomSettings(const int& l_radiusOfPath)
{
	m_radiusOfPath = l_radiusOfPath;
}

void HexagonGame::Setup(const sf::Vector2u& l_windowSize)
{
	m_windowSize = l_windowSize;

	//Player Sprite
	if (m_playerTex.loadFromFile("player.png")) {}
	m_player.setTexture(m_playerTex);
	m_player.setOrigin(8.f, 8.f);
	m_player.rotate(90.f);
	m_playerTex.setSmooth(true);

	//Middle Dot
	middle.setRadius(10.f);
	middle.setOrigin(middle.getRadius(), middle.getRadius());
	middle.setFillColor(sf::Color::Color(255, 0, 0, 100));

	

	m_radiusOfPath = 50;
	m_fSpeed = 100.f;

	kordinatni_sustav.translate(sf::Vector2f(m_windowSize.x / 2, m_windowSize.y / 2));
}