//--------------------------------//
// Author: Mihael Petricevic	  //
// Contact: miha53cevic@gmail.com //
// Date: 13.5.2019.				  //
//--------------------------------//

#include "mihaSimpleSFML.h"
#include <vector>

#define TRIANGLE_MODE 1

// Calculate half point between 2 points
// X = (x1 + x2) / 2
// Y = (y1 + y2) / 2
sf::Vector2f getHalfPoint(sf::Vector2f a, sf::Vector2f b)
{
	float x = (a.x + b.x) / 2;
	float y = (a.y + b.y) / 2;

	return {x, y};  
}

// Structure for representing a Pixel
// A pixel is a Point with X, Y Coordinates
// It has a defined RGB Colour
// It is represented as a sf::Vertex
struct Pixel
{
	Pixel()
		: m_pixel(sf::Points, 1)
	{
		m_pixel[0].position.x = 0;
		m_pixel[0].position.y = 0;
		m_pixel[0].color = sf::Color::White;
	}

	Pixel(float x, float y, sf::Color c = sf::Color::White)
		: m_pixel(sf::Points, 1) 
	{
		m_pixel[0].position.x = x;
		m_pixel[0].position.y = y;
		m_pixel[0].color = c;
	}

	sf::VertexArray m_pixel;	
};

class ChaosGameRevisited : public mihaSimpleSFML
{
public:
	ChaosGameRevisited() {}
	
private:
	std::vector<Pixel> vecPixels;
	Pixel current;

protected:	
	virtual bool OnUserCreate() override
	{
		// The game starts with main 3 random points
		// and a 4th main point
		for (int i = 0; i < 4; i++)
		{
			float x = rand() % ScreenWidth();
			float y = rand() % ScreenHeight();

			vecPixels.push_back(Pixel(x, y, sf::Color::Red));
		}

		// If triangle mode is enabled setup main points
		if (TRIANGLE_MODE)
		{
			vecPixels[0].m_pixel[0].position = sf::Vector2f(0, ScreenHeight() - 1);
			vecPixels[1].m_pixel[0].position = sf::Vector2f(ScreenWidth() - 1, ScreenHeight() - 1);
			vecPixels[2].m_pixel[0].position = sf::Vector2f(ScreenWidth() / 2, 0);
		}

		// Make the starting current point different colour
		current = vecPixels[3];
		current.m_pixel[0].color = sf::Color::Blue;
	
		return true;
	}
	virtual bool OnUserUpdate(sf::Time elapsed) override
	{
  		// Every update choose 1 of the 3 main random points
  		// and create a half point pixel and make it the current pixel
  		int random = rand() % 3;
		sf::Vector2f halfPoint = getHalfPoint(current.m_pixel[0].position, vecPixels[random].m_pixel[0].position);
  		vecPixels.push_back(Pixel(halfPoint.x, halfPoint.y));
  		current = vecPixels[vecPixels.size() - 1];
	
		// Draw Pixels
		for (const auto i : vecPixels)
		{
			Draw(i.m_pixel);
		}
	
		return true;
	}
};

int main()
{
	srand(time(0));

	ChaosGameRevisited app;
	app.Construct(640, 480, L"Chaos Game Revisited");
	app.Start();

	return 0;
}