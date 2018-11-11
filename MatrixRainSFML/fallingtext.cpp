#include "fallingtext.h"

FallingText::FallingText(sf::Vector2u l_windowSize) { Setup(l_windowSize , 50, 12); }
FallingText::~FallingText() {}

void FallingText::PrepeareStreamer(sStreamer* s)
{
	s->fPosition = 0.f;
	s->nStupac = (rand() % ((int)m_windowSize.x / m_FontSize)) * m_FontSize;
	s->fSpeed = rand() % 20 + 1;
	s->sText = L"ABCDEFGHIJKLMNOPQRSTWXYZ";
}

void FallingText::Update()
{
	//If offscreen reset pos
	for (auto &n : m_vecStreamers)
	{
		//Treba unsigned int pretvoriti u int sa (int) predznakom
		if ((int)n.fPosition - ((int)n.sText.size() * m_FontSize) >= (int)m_windowSize.y)
		{
			PrepeareStreamer(&n);
		}
	}
}

void FallingText::Draw(sf::RenderWindow& window)
{
	for (auto &n : m_vecStreamers)
	{
		for (int i = 0; i < n.sText.size(); i++)
		{
			//Ako je prva znamenka je bijela nakon nje su sljedece 3 sive
			if (i == 0)
				m_text.setFillColor(sf::Color::Color::White);
			else if (i <= 3)
				m_text.setFillColor(sf::Color::Color::Color(192, 192, 192, 255));
			//Ako je brzina spora stavi da su tamnije boje kao da su udaljeniji ako je brza su normalne zelene
			else if (n.fSpeed <= 5)
				m_text.setFillColor(sf::Color::Color(38, 127, 0, 255));
			else m_text.setFillColor(sf::Color::Color::Green);

			m_text.setPosition(n.nStupac, (int)n.fPosition - i * m_FontSize);
			m_text.setString(n.sText[i]);
			window.draw(m_text);
		}
		
		//Speed of falling text
		n.fPosition += n.fSpeed;
	}
}

void FallingText::Setup(sf::Vector2u l_windowSize, int l_MaxStreamers, int l_fontSize)
{
	m_FontSize = l_fontSize;
	m_windowSize = l_windowSize;

	//Create an array of streamers
	for (int i = 0; i < l_MaxStreamers; i++)
	{
		sStreamer s;
		PrepeareStreamer(&s);
		m_vecStreamers.push_back(s);
	}

	//Load Font
	if (m_font.loadFromFile("consola.ttf")) {}
	m_text.setFont(m_font);
	m_text.setCharacterSize(l_fontSize);
	m_text.setFillColor(sf::Color::Green);
}

void FallingText::MatrixSize(int l_MaxStreamerCount, int l_fontSize)
{
	m_vecStreamers.clear();
	Setup(m_windowSize, l_MaxStreamerCount, l_fontSize);
}