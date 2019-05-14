// Author: Mihael Petricevic
// Date: 14.5.2019.

#include "mihaSimpleSFML.h"
#include <cmath>

// Settings
float angle = 0;
float fSpeed = 100.0f;

// PI CONSTANT
const float PI = 3.141592;

// Converts degress to radians
float toRadian(float deg)
{
	return deg * (PI / 180);
}

class Logo : public mihaSimpleSFML
{
public:
	Logo() {}

private:
	sf::Sprite logo;
	sf::Texture logo_tex;
	sf::Vector2f velocity;

	bool OnUserCreate() override
	{
		// Setting up logo
		if (!logo_tex.loadFromFile("logo.png"))
		{
			// Error
			logo_tex.create(200, 200);			
		}

		logo.setTexture(logo_tex);
		logo.setPosition(ScreenWidth() / 2, ScreenHeight() / 2);

		// Starting vector
		velocity.x = cosf(toRadian(angle));
		velocity.y = sinf(toRadian(angle));
		
		return true;
	} 	

	bool OnUserUpdate(sf::Time elapsed) override
	{
		// Update position
		// s = v * t
		logo.move(velocity * fSpeed * elapsed.asSeconds());

		// Bounce of wall
		// X coordinate
		if (logo.getPosition().x < 0 || logo.getPosition().x + logo_tex.getSize().x > ScreenWidth() - 1)
		{
			velocity.x *= -1;
		}

		// Y coordinate
		if (logo.getPosition().y < 0 || logo.getPosition().y + logo_tex.getSize().y > ScreenHeight() - 1)
		{
			velocity.y *= -1;		
		}

		// Draw the logo
		Draw(logo);
	
		return true;
	}
};

int main(int argc, char* argv[])
{
	// Random num SEED
	srand(time(0));

	// Random angle
	angle = rand() % 360;
	
	// Setup settings
	if (argc == 2)	angle = std::stof(argv[1]);
	if (argc == 3)	angle = std::stof(argv[1]), fSpeed = std::stof(argv[2]);
	printf("Angle: %f	Speed: %f\n", angle, fSpeed);	
	
	Logo app;
	app.Construct(640, 480, L"Logo Hitting Corner");
	app.Start();

	return 0;
}
