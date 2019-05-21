//////////////////////////////////////////////////////////
/*
	@Author Mihael Petrièeviæ
	Date: 21.12.2018. 
*/
//////////////////////////////////////////////////////////

#include <SFML/Graphics.hpp>
#include <vector>
#include <list>

int Angle = 45;
const int nLength = 200;

//Cuva transformaciju od grane kod koje se trebaju granati lijeva i desna grana
std::list<sf::Transform> LastTrans;

void DrawLine(sf::RenderWindow* window, const std::vector<std::pair<sf::RectangleShape, sf::Transform>>& lines)
{
	for (auto const i : lines)
	{
		window->draw(i.first, i.second);
	}
}

void branch(int length, sf::RectangleShape& line, std::vector<std::pair<sf::RectangleShape, sf::Transform>>& vecLines, sf::Transform& trans)
{
	//Draw Line
	line.setFillColor(sf::Color::Black);
	line.setSize(sf::Vector2f(1, -length));
	line.setPosition(0, 0);

	//Push line into vector
	vecLines.push_back(std::make_pair(line, trans));

	if (length > 1)
	{
		//Sprema pocetnu poziciju na kojoj od koje se granaju lijeva i desna grana i prvo se crta desna grana
		LastTrans.push_front(trans);

		trans.translate(0, -length).rotate(Angle);
		branch(length * 0.67, line, vecLines, trans);

		//Uzima zadnju tj, pocetnu poziciju poziciju da napravi lijevu granu
		trans = LastTrans.front().translate(0, -length).rotate(-Angle);
		branch(length * 0.67, line, vecLines, trans);

		LastTrans.pop_front();
	}
};

int main()
{
	printf_s("A, D - mjenjaju kut pod kojim se grane granaju\nLAlt - mjenja brzinu tih promjena(po 1 ili 5 stupnjeva)\n");

	//======== Variable Declarations ========//
	int nWidth = 1280;
	int nHeight = 720;

	int AngleChangeSpeed = 1;

	sf::Transform trans;
	std::vector<std::pair<sf::RectangleShape, sf::Transform>> vecLines;

	sf::RectangleShape line;

	//======== Setup ========//
	trans.translate(nWidth / 2, nHeight);

	branch(nLength, line, vecLines, trans);

	auto Restart = [&]() -> void
	{
		trans = sf::Transform();

		trans.translate(nWidth / 2, nHeight);
		vecLines.clear();

		branch(nLength, line, vecLines, trans);

		printf_s("\nTrenutni Kut: %d", Angle);
	};

	//======== SFML Game Loop ========//
	sf::RenderWindow window(sf::VideoMode(nWidth, nHeight), "Fractal Trees SFML");

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			//Keyboard input
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::LAlt)
			{
				if (AngleChangeSpeed == 1)
				{
					AngleChangeSpeed = 5;
				}
				else AngleChangeSpeed = 1;
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::A)
			{
				Angle -= AngleChangeSpeed;
				Restart();
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::D)
			{
				Angle += AngleChangeSpeed;
				Restart();
			}
		}

		//======== Drawing ========//
		window.clear(sf::Color::White);

		DrawLine(&window, vecLines);

		window.display();
	}

	return 0;
}