#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

int main()
{
	// ========= Variables ========= //
	const int nScreenWidth = 1280;
	const int nScreenHeight = 720;

	struct Star
	{
		Star()
		{
			x = rand() % nScreenWidth;
			y = rand() % nScreenHeight;
			z = 0;

			circle.setPosition(x, y);
			
		}

		float x, y, z;
		float px, py;
		const float fSpeed = 1.05f;

		sf::CircleShape circle;
		sf::Vertex line[2];

		void update()
		{
			//Prijasnje tocke x-a i y-a
			px = x;
			py = y;

			//Radijus kruznica povecava se s vremenom
			z += 0.01f;

			//Formula koju ne razumijem ali dela
			x = (x - nScreenWidth / 2) * fSpeed + nScreenWidth / 2;
			y = (y - nScreenHeight / 2) * fSpeed + nScreenHeight / 2;

			circle.setRadius({ z });
			circle.setPosition(x, y);

			//Stvaranje linije izmedu stare i nove pozicije
			line[0] = sf::Vertex(sf::Vector2f(px, py));
			line[1] = sf::Vertex(sf::Vector2f(x, y));

			//Ako je izvan ekrana resetiraj
			if (x < 0 || x > nScreenWidth || y < 0 || y > nScreenHeight)
			{
				z = 0;
				x = rand() % nScreenWidth;
				y = rand() % nScreenHeight;
			}
		}
	};

	std::vector<Star> vecStar(200);

	// ========= WINDOW ========= //
	sf::RenderWindow window(sf::VideoMode(nScreenWidth, nScreenHeight), "StarField SFML");
	window.setVerticalSyncEnabled(true);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// ========= LOGIC ========= //
		for (auto &i : vecStar)
		{
			i.update();
		}

		// ========= DRAWING ========= //
		window.clear(sf::Color::Black);
		
		for (auto &i : vecStar)
		{
			window.draw(i.circle);
			window.draw(i.line, 2, sf::Lines);
		}

		window.display();
	}

	return 0;
}