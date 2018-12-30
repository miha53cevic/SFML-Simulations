//////////////////////////////////////////////////////////
/*
	@Author Mihael Petrièeviæ
	Date: 30.12.2018.
*/
//////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <math.h>

#include "mihaSimpleSFML.h"

// Globals
bool bSnowflake = false;
float Size = 2.0f;

struct Line
{
	Line(sf::Color c = sf::Color::White)
	{
		line.setFillColor(c);
	}

	sf::RectangleShape line;
	sf::Transform trans;

	std::vector<Line> Generate()
	{
		std::vector<Line> lines(4);

		lines[0].trans = trans;
		lines[0].line.setSize(sf::Vector2f(line.getSize().x / 3.0f, 1.0f));
		lines[0].line.setPosition(line.getPosition().x, line.getPosition().y);

		lines[1].trans = trans;
		lines[1].trans.translate(line.getPosition().x + line.getSize().x / 3.0f, line.getPosition().y).rotate(-60);
		lines[1].line.setSize(sf::Vector2f(line.getSize().x / 3.0f, 1.0f));

		float Height = (line.getSize().x / 3 ) * std::sqrt(3) / 2;

		lines[2].trans = trans;
		lines[2].trans.translate(line.getPosition().x + line.getSize().x * 0.5f, line.getPosition().y - Height).rotate(60);
		lines[2].line.setSize(sf::Vector2f(line.getSize().x / 3.0f, 1.0f));

		lines[3].trans = trans;
		lines[3].line.setSize(sf::Vector2f(line.getSize().x / 3.0f, 1.0f));
		lines[3].line.setPosition(line.getPosition().x + line.getSize().x / 3.0f * 2.0f, line.getPosition().y);

		return lines;
	}
};

class demo : public mihaSimpleSFML
{
public:
	demo()
	{}

private:
	std::vector<Line> vecLines;

	Line newLine;

protected:
	virtual bool OnUserCreate()
	{
		//Draw Line or snowflake
		if (!bSnowflake)
		{
			// Starting line
			newLine.line.setSize(sf::Vector2f(ScreenWidth(), 1));
			newLine.line.setPosition(-ScreenWidth() / 2, 0);
			newLine.trans.translate(ScreenWidth() / 2, ScreenHeight() / 2);

			for (auto &i : newLine.Generate())
			{
				vecLines.push_back(i);
			}
		}
		else
		{
			Line newLines[3];

			for (int i = 0; i < 3; i++)
			{
				newLines[i].trans.translate(ScreenWidth() / 2, ScreenHeight() / 4);
				newLines[i].line.setSize(sf::Vector2f(ScreenWidth() / Size, 1));
			}

			float Height = newLines[0].line.getSize().x * std::sqrt(3) / 2;

			newLines[0].line.setPosition(-ScreenWidth() / Size / 2, 0);

			newLines[1].trans.translate(0, Height).rotate(-120);
			newLines[1].line.setPosition(0, 0);
			
			newLines[2].trans.translate(ScreenWidth() / Size / 2, 0).rotate(120);
			newLines[2].line.setPosition(0, 0);

			vecLines.push_back(newLines[0]);
			vecLines.push_back(newLines[1]);
			vecLines.push_back(newLines[2]);
		}

		return true;
	}

	virtual bool OnUserUpdate(sf::Time elapsed)
	{
		sf::Event event;

		while (getWindow()->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_bDone = true;

			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Space)
			{
				std::vector<Line> vecNewLines;

				for (auto &i : vecLines)
				{
					for (auto &j : i.Generate())
					{
						vecNewLines.push_back(j);
					}
				}

				vecLines.clear();

				vecLines = vecNewLines;

				vecNewLines.clear();
			}
		}

		for (auto &i : vecLines)
		{
			Draw(i.line, i.trans);
		}

		return true;
	}

};

int main()
{
	int choice;

	std::cout << "Koch Fractal Snowflake by Mihael Petricevic, 30.12.2018.\n\n\nControls: Space\n";
	std::cout << "1. Line\n2. Snowflake\n\nChoice: ";
	std::cin >> choice;

	if (choice == 1)
		bSnowflake = false;
	else bSnowflake = true;

	if (bSnowflake)
	{
		std::cout << "Unesi velicinu od 1.0f nadalje: ";
		std::cin >> Size;

		if (Size <= 0.0f)
			Size = 1.0f;
	}

	demo Test;
	Test.Construct(1280, 800, L"Koch Fractal Snowflake SFML");
	Test.Start();

	return 0;
}