//////////////////////////////////////////////////////////
/*
	@Author Mihael Petrièeviæ
	Date: 23.12.2018.
*/
//////////////////////////////////////////////////////////

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <vector>
#include <iostream>

const int ScreenWidth = 1280;
const int ScreenHeight = 720;

const int PointThickness = 5;

struct Point
{
	Point(sf::Color c = sf::Color::Black)
	{
		point.setRadius(PointThickness);
		point.setFillColor(c);
		point.setOrigin(sf::Vector2f(PointThickness / 2, PointThickness / 2));
		point.setPosition(sf::Vector2f(rand() % ScreenWidth, rand() % ScreenHeight));
	}

	sf::CircleShape point;

	void ChangePos(const int& x, const int& y)
	{
		point.setPosition(x, y);
	}
};

void betweenPoint(std::vector<Point>& vecPoints, Point& r, const Point& abc, sf::Color c)
{
	sf::Vector2f newPointPos;
	newPointPos.x = (r.point.getPosition().x + abc.point.getPosition().x) / 2;
	newPointPos.y = (r.point.getPosition().y + abc.point.getPosition().y) / 2;

	Point newPoint(c);
	newPoint.ChangePos(newPointPos.x, newPointPos.y);

	//r postaje novi point
	r.point.setPosition(newPointPos.x, newPointPos.y);

	vecPoints.push_back(newPoint);
}

int main()
{
	srand(time(NULL));

	bool PerfectTriangle = false;

	printf_s("Normal Triangle: 0/1 : ");
	std::cin >> PerfectTriangle;

	Point a, b, c, r(sf::Color::Red), orR;
	orR = r;

	std::vector<Point> vecPoints;

	if (PerfectTriangle)
	{
		a.ChangePos(0, ScreenHeight);
		b.ChangePos(ScreenWidth, ScreenHeight);
		c.ChangePos(ScreenWidth / 2, 0);
	}

	sf::RenderWindow window(sf::VideoMode(ScreenWidth, ScreenHeight), "Chaos Game SFML");
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		int random = rand() % 3 + 1;

		switch (random)
		{
			case 1:
			{
				betweenPoint(vecPoints, r, a, sf::Color::Blue);
				break;
			}
			case 2:
			{
				betweenPoint(vecPoints, r, b, sf::Color::Green);
				break;
			}
			case 3:
			{
				betweenPoint(vecPoints, r, c, sf::Color::Yellow);
				break;
			}
		}

		window.clear(sf::Color::White);

		for (auto const i : vecPoints)
		{
			window.draw(i.point);
		}

		window.draw(a.point);
		window.draw(b.point);
		window.draw(c.point);

		window.draw(orR.point);

		window.display();
	}

	return 0;
}