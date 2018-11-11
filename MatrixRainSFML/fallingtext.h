#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class FallingText
{
public:
	FallingText(sf::Vector2u l_windowSize);
	~FallingText();

	void Update();
	void Draw(sf::RenderWindow& window);

	void MatrixSize(int l_MaxStreamerCount, int l_fontSize);

private:
	
	struct sStreamer
	{
		float fPosition;
		int nStupac;
		float fSpeed;

		std::wstring sText;
	};

	int m_MaxStreamers;
	int m_FontSize;

	sf::Vector2u m_windowSize;

	std::vector<sStreamer> m_vecStreamers;

	sf::Text m_text;
	sf::Font m_font;

	void PrepeareStreamer(sStreamer* s);
	void Setup(sf::Vector2u l_windowSize, int l_nMaxStreamers, int l_fontSize);
};