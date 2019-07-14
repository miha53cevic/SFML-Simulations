/*
// Author: Mihael Petricevic
// Contact: miha53cevic@gmail.com
// Date: 14.7.2019.
//
// Sources: Wikipedia
//			https://en.wikipedia.org/wiki/Bowyer%E2%80%93Watson_algorithm
//
*/

#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>

float distance(sf::Vector2f a, sf::Vector2f b)
{
	return std::sqrtf(powf(b.x - a.x, 2) + powf(b.y - a.y, 2));
}

struct Edge
{
	Edge() {}
	Edge(sf::Vector2f a, sf::Vector2f b)
	{
		points[0] = a;
		points[1] = b;

		length = distance(a, b);
	}

	bool operator== (Edge a)
	{
		if (this->points[0] == a.points[0] && this->points[1] == a.points[1])
			return true;
		else if (this->points[0] == a.points[1] && this->points[1] == a.points[0])
			return true;
		else return false;
	}

	float length;
	sf::Vector2f points[2];

	bool isBad = false;

	sf::Vector2f intersection(Edge a)
	{
		const float a1 = this->points[1].y - this->points[0].y;
		const float b1 = this->points[0].x - this->points[1].x;
		const float c1 = a1 * (this->points[0].x) + b1 * this->points[0].y;

		const float a2 = a.points[1].y - a.points[0].y;
		const float b2 = a.points[0].x - a.points[1].x;
		const float c2 = a2 * (a.points[0].x) + b2 * a.points[0].y;

		float determinant = a1 * b2 - a2 * b1;

		if (determinant == 0)
		{
			/*std::cout << "Lines are paralel\n";
			std::cout << "P1 = " << "{" << this->points[0].x << this->points[0].y << "}" << " - " << "{" << this->points[1].x << this->points[1].y << "}" << "\n";
			std::cout << "P2 = " << "{" << a.points[0].x << a.points[0].y << "}" << " - " << "{" << a.points[1].x << a.points[1].y << "}" << "\n";*/

			return { INFINITY, INFINITY };
		}

		else
		{
			float x = (b2*c1 - b1 * c2) / determinant;
			float y = (a1*c2 - a2 * c1) / determinant;
			return { x, y };
		}
	}
};

class Triangle
{
public:
	Triangle() {}
	Triangle(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c)
	{
		m_points[0] = a;
		m_points[1] = b;
		m_points[2] = c;

		m_edges[0] = Edge(m_points[0], m_points[1]);
		m_edges[1] = Edge(m_points[1], m_points[2]);
		m_edges[2] = Edge(m_points[2], m_points[0]);
	}

	bool operator== (Triangle a)
	{
		if (this->m_points[0] == a.m_points[0] && this->m_points[1] == a.m_points[1] && this->m_points[2] == a.m_points[2])
			return true;
		else return false;
	}

	void SetupOverPoints(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c)
	{
		m_points[0] = a;
		m_points[1] = b;
		m_points[2] = c;

		m_edges[0] = Edge(m_points[0], m_points[1]);
		m_edges[1] = Edge(m_points[1], m_points[2]);
		m_edges[2] = Edge(m_points[2], m_points[0]);
	}

	float getArea()
	{
		float a = m_edges[0].length;
		float b = m_edges[1].length;
		float c = m_edges[2].length;

		float s = (a + b + c) / 2;

		return sqrtf(s * (s - a) * (s - b) * (s - c));
	}

	bool isPointInCircumcircle(sf::Vector2f point)
	{
		sf::Vector2f a = m_edges[0].points[0];
		sf::Vector2f b = m_edges[0].points[1];
		sf::Vector2f c = m_edges[1].points[0];
		sf::Vector2f d = m_edges[1].points[1];

		Edge okomica1(calculateOkomica(a, midPoint(a, b)), midPoint(a, b));
		Edge okomica2(calculateOkomica(c, midPoint(c, d)), midPoint(c, d));

		sf::Vector2f circumcenter = okomica1.intersection(okomica2);

		float radius = (m_edges[0].length * m_edges[1].length * m_edges[2].length) / (4 * getArea());

		return distance(point, circumcenter) <= radius;
	}

	bool isBad = false;

	sf::Vector2f getPoint(int index) { return m_points[index]; }
	Edge getEdge(int index) { return m_edges[index]; }

private:
	Edge m_edges[3];
	sf::Vector2f m_points[3];

	sf::Vector2f midPoint(sf::Vector2f a, sf::Vector2f b)
	{
		float x = (a.x + b.x) / 2;
		float y = (a.y + b.y) / 2;

		return { x, y };
	}

	sf::Vector2f calculateOkomica(sf::Vector2f a, sf::Vector2f b)
	{
		// use b as the midpoint, new line is centered on b
		float dx = b.x - a.x;
		float dy = b.y - a.y;

		float a1 = b.x - dy / 2;
		float b1 = b.y + dx / 2;

		return { a1, b1 };
	}
};