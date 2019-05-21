#pragma once
#include <SFML/Graphics.hpp>
class lineStrip
{
public:
	lineStrip()
		: m_lineStrip(sf::PrimitiveType::LinesStrip)
	{}

	sf::VertexArray* getLineStrip() { return &m_lineStrip; }

private:
	sf::VertexArray m_lineStrip;
};

class line
{
public:
	line(sf::Vector2f a, sf::Vector2f b, sf::Color c)
		: m_line(sf::PrimitiveType::Lines)
	{
		m_line.append(sf::Vertex(a, c));
		m_line.append(sf::Vertex(b, c));
	}

	sf::VertexArray* getLine() { return &m_line; }

private:
	sf::VertexArray m_line;
};