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
			// Tocka ne smije biti na sredini ekrana jer ce onda tamo (0, 0)
			// ostati zaglavljena jer 0 * fSpeed je uvijek 0
			do
			{
			    x = rand() % nScreenWidth;
			    y = rand() % nScreenHeight;
			    	
			} while(x == nScreenWidth / 2 && y == nScreenHeight / 2);
			
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

			// Stavi kordinate u normalni kordinatni sustav -> srediste (0, 0)
			// Pomnozi ih sa nekim multiplierom jer u normalnom kordinatnom sustavu
			// mnozenjem se kordinate uvijek priblizavaju rubu kordinatnog sustava ovisno
			// gdje se pocetno nalaze.
			// Zatim nakon te multiplikacije vrati u nas kordinatni sustav -> srediste (ScreenWidth / 2, ScreenHeight / 2)
			x = (x - nScreenWidth  / 2) * fSpeed + nScreenWidth  / 2;
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
				
				// Pogledaj konstruktor za objasnjenje
				do
				{
				    x = rand() % nScreenWidth;
				    y = rand() % nScreenHeight;
				    	
				} while(x == nScreenWidth / 2 && y == nScreenHeight / 2);
				
				
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