#include <SFML/Graphics.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

int main()
{
	const int nScreenWidth = 1280;
	const int nScreenHeight = 768;

	double A = 1, B = 1, C = 0, D = 0;

	printf_s("Controls:\tMove window ---> A, D, W, S\n\t\tZoom in and out ---> UP, DOWN\n\t\tReset view ---> SPACE\n\t\tEscape ---> Izaberi novu formulu!\n\n");
	printf_s("Amplituda = ");
	scanf_s("%lf", &A);
	printf_s("B = ");
	scanf_s("%lf", &B);
	printf_s("C = ");
	scanf_s("%lf", &C);
	printf_s("D = ");
	scanf_s("%lf", &D);

	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;

	sf::RenderWindow window(sf::VideoMode(nScreenWidth, nScreenHeight), "SFML Sinusoida", sf::Style::Default, settings);

	//	View za panning i zooming
	sf::View view = window.getView();

	//	Flip the y - axis
	view.setSize(nScreenWidth, -nScreenHeight);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			//	Ako je ESC kliknut nudi izbor za upis nove formule
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			{
				window.close();
				main();
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::W)
				view.move(sf::Vector2f(0.f, +10.f));
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::S)
				view.move(sf::Vector2f(0.f, -10.f));
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::A)
				view.move(sf::Vector2f(-10.f, 0.f));
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::D)
				view.move(sf::Vector2f(+10.f, 0.f));

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Up)
				view.zoom(0.5f);
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Down)
				view.zoom(1.5f);
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Space)
			{
				view = window.getDefaultView();
				view.setSize(nScreenWidth, -nScreenHeight);
			}
		}
		
		///////////////////////////////////////////////////////
		/*
								Logic
		*/
		///////////////////////////////////////////////////////
		sf::VertexArray sinusoida(sf::PrimitiveType::LineStrip);

		//	Stavlja da je 0,0 u sredini ekrana kao u kordinatnom sustavu
		sf::Transform transform;
		transform.translate(sf::Vector2f(nScreenWidth / 2, nScreenHeight / 2));

		for (int x = -nScreenWidth / 2; x < nScreenWidth / 2; x++)
		{
			sinusoida.append(sf::Vertex(sf::Vector2f(x, A * sin(B * x + C) + D)));
		}

		sf::VertexArray osX(sf::PrimitiveType::Lines, 2);
		sf::VertexArray osY(sf::PrimitiveType::Lines, 2);

		osY[0] = sf::Vector2f(nScreenWidth / 2, 0);
		osY[1] = sf::Vector2f(nScreenWidth / 2, nScreenHeight);

		osX[0] = sf::Vector2f(0, nScreenHeight / 2);
		osX[1] = sf::Vector2f(nScreenWidth, nScreenHeight / 2);

		//	Oboji linije u sivu
		osX[0].color = sf::Color::Color(125, 125, 125);
		osX[1].color = sf::Color::Color(125, 125, 125);

		osY[0].color = sf::Color::Color(125, 125, 125);
		osY[1].color = sf::Color::Color(125, 125, 125);

		///////////////////////////////////////////////////////
		/*
								Other
		*/
		///////////////////////////////////////////////////////

		//	Background square
		sf::RectangleShape background;
		background.setSize(sf::Vector2f(nScreenWidth, nScreenHeight));
		background.setFillColor(sf::Color::Black);

		///////////////////////////////////////////////////////
		/*
								Drawing
		*/
		///////////////////////////////////////////////////////
		window.setView(view);

		window.clear(sf::Color::Cyan);
		
		window.draw(background);
		window.draw(osX);
		window.draw(osY);
		window.draw(sinusoida, transform);

		window.display();
	}

	return 0;
}