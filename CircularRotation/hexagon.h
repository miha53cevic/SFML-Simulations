#pragma once
#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <cmath>

class HexagonGame
{
public:
	HexagonGame(const sf::Vector2u& l_windowSize);
	~HexagonGame();

	void Update(sf::Time elapsed);
	void Draw(sf::RenderWindow& window);
	void Input();

	void CustomSettings(const int& l_raidusOfPath);

private:
	sf::Vector2u m_windowSize;

	sf::Transform kordinatni_sustav;

	float m_degrees;
	int m_radiusOfPath;
	float m_fSpeed;

	bool m_bLeftPressed;
	bool m_bRightPressed;

	sf::RectangleShape hexagon[6];
	sf::CircleShape middle;
	sf::Texture m_playerTex;
	sf::Sprite m_player;

	void Setup(const sf::Vector2u& l_windowSize);
	float toRad();
};
