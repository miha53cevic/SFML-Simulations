//////////////////////////////////////////////////////////
/*
	@Author Mihael Petrièeviæ
	Date: 22.12.2018.
*/
//////////////////////////////////////////////////////////

#include <SFML/Graphics.hpp>
#include <math.h>
#include <vector>
#include <list>

struct Branch
{
	sf::RectangleShape rect;
	sf::VertexArray triangle;
	sf::Transform trans;
};

const int Angle = 60;
const int nLength = 200;
int RectSize = 10;
int LengthLimit = 1;

//Cuva transformaciju od grane kod koje se trebaju granati lijeva i desna grana
std::list<sf::Transform> LastTrans;

void DrawLine(sf::RenderWindow* window, const std::vector<Branch>& branches)
{
	for (auto const i : branches)
	{
		window->draw(i.rect,		i.trans);
		window->draw(i.triangle,	i.trans);
	}
}

void branch(int length, sf::RectangleShape& rect, sf::VertexArray& triangle, std::vector<Branch>& vecBranches, sf::Transform& trans)
{
	//Draw Rect
	rect.setFillColor(sf::Color::Black);
	rect.setSize(sf::Vector2f(RectSize, -length));
	rect.setPosition(0, 0);

	//Draw Triangle
	triangle[0].color = sf::Color::Green;
	triangle[1].color = sf::Color::Green;
	triangle[2].color = sf::Color::Green;

	float h = (RectSize * sqrt(3)) / 2.f;

	triangle[0].position = sf::Vector2f(0, -length);
	triangle[1].position = sf::Vector2f(RectSize, -length);
	triangle[2].position = sf::Vector2f(RectSize / 2, -length - h);

	//Push line into vector
	vecBranches.push_back({ rect, triangle, trans });

	if (length > LengthLimit)
	{
		//Sprema pocetnu poziciju na kojoj od koje se granaju lijeva i desna grana i prvo se crta desna grana
		LastTrans.push_front(trans);
		
		trans.translate(RectSize / 2, -length - h).rotate(Angle);
		branch(length * 0.67, rect, triangle, vecBranches, trans);

		//Uzima zadnju tj, pocetnu poziciju poziciju da napravi lijevu granu
		trans = LastTrans.front().translate(0, -length).rotate(-Angle);
		branch(length * 0.67, rect, triangle, vecBranches, trans);

		LastTrans.pop_front();
	}
};

int main()
{
	//======== Variable Declarations ========//
	int nWidth = 1280;
	int nHeight = 720;

	int AngleChangeSpeed = 1;

	sf::Transform trans;
	std::vector<Branch> vecBranches;

	sf::RectangleShape rect;
	sf::VertexArray triangle(sf::Triangles, 3);

	//======== Setup ========//
	trans.translate(nWidth / 2, nHeight);

	branch(nLength, rect, triangle, vecBranches, trans);

	auto Restart = [&]() -> void
	{
		trans = sf::Transform();

		trans.translate(nWidth / 2, nHeight);
		vecBranches.clear();

		branch(nLength, rect, triangle, vecBranches, trans);
	};

	//======== SFML Game Loop ========//
	sf::RenderWindow window(sf::VideoMode(nWidth, nHeight), "Pitagorino Stablo SFML");

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::W)
			{
				RectSize++;
				Restart();
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::S)
			{
				RectSize--;

				if (RectSize <= 0)
					RectSize = 1;

				Restart();
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::A)
			{
				LengthLimit--;

				if (LengthLimit <= 0)
					LengthLimit = 1;

				Restart();
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::D)
			{
				LengthLimit++;
				Restart();
			}
		}

		//======== Drawing ========//
		window.clear(sf::Color::White);

		DrawLine(&window, vecBranches);

		window.display();
	}

	return 0;
}