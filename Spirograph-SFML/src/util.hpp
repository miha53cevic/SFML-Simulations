#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>

class line
{
public:
    line(float x1, float y1, float x2, float y2, sf::Color c = sf::Color::White)
        : points(sf::Lines) 
    {
        points.append(sf::Vertex(sf::Vector2f(x1, y1), c));
        points.append(sf::Vertex(sf::Vector2f(x2, y2), c));
    }

    sf::VertexArray getLine()
    {
        return points;
    }

private:
    sf::VertexArray points;
};

class point
{
public:
    point(float x, float y, sf::Color c = sf::Color::White)
        : points(sf::Points) 
    {
        points.append(sf::Vertex(sf::Vector2f(x, y), c));
    }

    sf::VertexArray getPoint()
    {
        return points;
    }

private:
    sf::VertexArray points;
};