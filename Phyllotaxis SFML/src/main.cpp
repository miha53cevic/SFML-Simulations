//////////////////////////////////////////////////////////
/*
	@Author Mihael Petrièeviæ
	Date: 1.1.2019.
*/
//////////////////////////////////////////////////////////

#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <math.h>

#include "mihaSimpleSFML.h"

// Globals
int angle;
int n;
int c;
int r;
int Radius;
int SpecialAngle;

char Edit;
// End of globals

struct Point
{
	Point(float r, sf::Color c = sf::Color::Black)
	{
		circle.setRadius(r);
		circle.setFillColor(c);
	}

	sf::CircleShape circle;
};

class demo : public mihaSimpleSFML
{
public:
	demo() {}

private:
	std::vector<Point> vecPoints;

	double ConvertToRadian(int DEGRESS)
	{
		return (DEGRESS * M_PI) / 180;
	}

protected:
	virtual bool OnUserCreate()
	{
		setBackgroundColor(sf::Color::White);

		if (Edit == 'n')
		{
			SpecialAngle = 137.5;
			n = 0;
			c = 8;
			Radius = 5;
		}

		return true;
	}

	virtual bool OnUserUpdate(sf::Time elapsed)
	{
		angle = n * SpecialAngle;
		r = c * std::sqrt(n);

		double x = std::cos(ConvertToRadian(angle)) * r + ScreenWidth() / 2;
		double y = std::sin(ConvertToRadian(angle)) * r + ScreenHeight() / 2;

		Point point(Radius, sf::Color::Color(rand() % 256, rand() % 256, rand() % 256));
		point.circle.setPosition(x, y);

		vecPoints.push_back(point);

		for (auto &i : vecPoints)
		{
			Draw(i.circle);
		}

		n++;

		return true;
	}

};

int main()
{
	std::cout << "Restart - SPACE\n\n";
	std::cout << "Edit variables y/n : ";
	std::cin >> Edit;

	if (Edit == 'y')
	{
		std::cout << "C (Udaljenost izmedu tocaka) = ";
		std::cin >> c;

		std::cout << "R (Radijus) = ";
		std::cin >> Radius;

		std::cout << "SpecialAngle (Default: 137.5): ";
		std::cin >> SpecialAngle;
	}

	demo Test;
	Test.Construct(1280, 720, L"Phyllotaxis SFML");
	Test.Start();

	return 0;
}