/*
//
// Author: Mihael Petričević, miha53cevic@gmail.com
// Date: 12.5.2019.
//
// Compile command used:
// g++ -std=c++17 -o app main.cpp $(pkg-config --libs --cflags sfml-graphics)
//
// SFML VER: 2.5.0
//
*/

#include "mihaSimpleSFML.h"
#include <vector>
#include <cmath>

// MACROS
// RAY_SPACING is the spacing between rays default is 10 set to 1 for more rays
// NUM_OBSTACLES set the number of random generated obstacled default 6
// BORDERS can be 0 or 1 - do you want to draw borders? 0/1 | y/n

#define RAY_SPACING 10
#define NUM_OBSTACLES 6
#define BORDERS 0

// PI CONSTANT
const float PI = 3.141592;

// Converts degress to radians
float toRadian(float deg)
{
	return deg * (PI / 180);
}

// Gets the distance between 2 points
float distance(float x1, float y1, float x2, float y2)
{
	return sqrtf(powf(x2 - x1, 2) + powf(y2 - y1, 2));
}

// Line class used for drawing a line using 2 points P1(x1, y1) and P2(x2, y2)
// Default color of the line is white
struct line
{
	line(float x1, float y1, float x2, float y2, sf::Color c = sf::Color::White)
		: pointArray(sf::Lines)
	{
		pointArray.append(sf::Vertex(sf::Vector2f(x1, y1), c));
		pointArray.append(sf::Vertex(sf::Vector2f(x2, y2), c));
	}

	sf::VertexArray pointArray;
};

// Ray class used for drawing a ray - vector that has a starting point and an angle
// To draw it, first translate to pos(x, y) then line to T( cos(angle), sin(angle) )
struct Ray
{
	Ray(float x, float y, float angle)
		: ray(0, 0, cosf(angle)*10, sinf(angle)*10) // x10 da se bolje vide dok su samostalne, tj. dok nema prepreke ili zida
	{
		// Translate coordinate system so that x,y are 0,0
		trans.translate(x, y);

		pos.x = x;
		pos.y = y;
		dir.x = cosf(angle)*10;
		dir.y = sinf(angle)*10;
	}

	line ray;
	sf::Transform trans;

	sf::Vector2f pos;
	sf::Vector2f dir;

	// Checks if ray intersects with wall and if it does returns the point at which they intersect
	// Uses line to line intersection using u and t paramaters
	sf::Vector2f cast(line wall)
	{
		// wall points T1 (x1, y1), T2 (x2, y2)
		const float x1 = wall.pointArray[0].position.x;
		const float y1 = wall.pointArray[0].position.y;
		const float x2 = wall.pointArray[1].position.x;
		const float y2 = wall.pointArray[1].position.y;

		// Ray points
		const float x3 = this->pos.x;
		const float y3 = this->pos.y;
		const float x4 = this->pos.x + this->dir.x;
		const float y4 = this->pos.y + this->dir.y;

		// Oboje jednadzbe koriste isti djeljenik
		const float djeljenik = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

		// Division by zero case
		if (djeljenik == 0)
			return sf::Vector2f(INFINITY, INFINITY);

		// Calculatin t and u
		// source: https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection
		const float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / djeljenik;
		const float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / djeljenik;

		// If intersection exists send back that point
		// Otherwise send back vector with x and y equaling INFINITY
		if (t > 0 && t < 1 && u > 0)	// nema u < 1 jer je polupravac u jednom smjeru
		{
			sf::Vector2f point;
			point.x = (x1 + t * (x2 - x1));
			point.y = (y1 + t * (y2 - y1));

			return point;
		}
		else return sf::Vector2f(INFINITY, INFINITY);
	}
};

// Particle class used for drawing a particle point that contains a 360 degress raycasting range
// and looks for walls and if it finds one connect a dot to it like a source of light
struct Particle
{
	// Initialization
	void Setup(float x, float y)
	{
		rays.clear();

		// Starting pos
		this->pos.x = x;
		this->pos.y = y;

		// Setup 360 degress of rays around mouse position point
		for (int i = 0; i < 360; i += RAY_SPACING)
		{
			rays.push_back(Ray(pos.x, pos.y, toRadian(i)));
		}
	}

	std::vector<Ray> rays;
	sf::Vector2f pos;

	// Check if any of the rays and walls intersect and if they do
	// Connect those points and draw a line representing them
	void updateRays(const std::vector<line>& walls)
	{
		for (auto &j : rays)
		{
			// Keeps track of the closest obstacle to the ray
			float closest = INFINITY;

			for (auto &i : walls)
			{
				// Intersection point between the ray and the obstacle
				sf::Vector2f pt;
				pt = j.cast(i);

				// Distance between the intersection point and the ray
				float dist = distance(pt.x, pt.y, j.pos.x, j.pos.y);

				if(dist < closest)
				{
					closest = dist;

					if (pt.x != INFINITY)
					{
						j.ray.pointArray[1].position = pt - j.pos;
					}
					else continue;
				}
			}
		}
	}

	void updatePos(sf::RenderWindow* window)
	{
		sf::Vector2i mouse = sf::Mouse::getPosition(*window);
		Setup(mouse.x, mouse.y);
	}
};

// Main program class initialized as a child of mihaSimpleSFML
class Raycasting2D : public mihaSimpleSFML
{
public:
	Raycasting2D()
	{}

private:
	// Contains all of the obstacles / walls
	std::vector<line> walls;

	// Main point from which the rays are drawn from
	Particle particle;

protected:
	virtual bool OnUserCreate()
	{
		// Add obstacles
		for (int i = 0; i < NUM_OBSTACLES; i++)
		{
			int x1 = rand() % ScreenWidth();
			int y1 = rand() % ScreenHeight();
			int x2 = rand() % ScreenWidth();
			int y2 = rand() % ScreenHeight();

			walls.push_back(line(x1, y1, x2, y2));
		}

		// Borders
		if (BORDERS)
		{
			walls.push_back(line(0, 0, ScreenWidth() - 1, 0));
			walls.push_back(line(0, 0, 0, ScreenHeight() - 1));
			walls.push_back(line(0, ScreenHeight() - 1, ScreenWidth() - 1, ScreenHeight() - 1));
			walls.push_back(line(ScreenWidth() - 1, 0, ScreenWidth() - 1, ScreenHeight() - 1));
		}

		// Initialize particle
		particle.Setup(ScreenWidth() / 2, ScreenHeight() / 2);

		return true;
	}

	virtual bool OnUserUpdate(sf::Time elapsed)
	{
		// Draw obstacles
		for (auto &i : walls)
		{
			Draw(i.pointArray);
		}

		// Update rays
		particle.updateRays(walls);

		// Draw rays
		for (auto& i : particle.rays)
		{
			Draw(i.ray.pointArray, i.trans);
		}

		// Change particle position to mouse position
		particle.updatePos(getWindow());

		return true;
	}
};

// Beginning of the program
int main()
{
	// Setting random number seed
	srand(time(0));

	Raycasting2D app;
	app.Construct(640, 480, L"2DRaycasting SFML");
	app.Start();

	return 0;
}
