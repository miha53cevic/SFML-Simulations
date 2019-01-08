#include <iostream>

#include "mihaSimpleSFML.h"
#include "Math.h"

class demo : public mihaSimpleSFML
{
public:
	demo() {}

private:
	std::wstring map;
	sf::RectangleShape block;
	
	sf::Vector2i BlockSize;
	int MapSizeX;
	int MapSizeY;

	sf::CircleShape ball;

	float fSpeed;

	float BallDX;
	float BallDY;
	float fAngle;

protected:
	virtual bool OnUserCreate()
	{
		MapSizeX = 16;
		MapSizeY = 8;

		BlockSize.x = ScreenWidth() / MapSizeX;
		BlockSize.y = ScreenHeight() / MapSizeY;

		map += L"################";
		map += L"#..............#";
		map += L"#..............#";
		map += L"#..............#";
		map += L"#..............#";
		map += L"#..............#";
		map += L"#..............#";
		map += L"################";

		ball.setFillColor(sf::Color::Yellow);
		ball.setRadius(20.0f);
		ball.setOrigin(ball.getRadius(), ball.getRadius());
		ball.setPosition(ScreenWidth() / 2, ScreenHeight() / 2);

		block.setFillColor(sf::Color::Color(125, 125, 125));
		block.setSize(sf::Vector2f(BlockSize.x, BlockSize.y));

		fSpeed = 200.0f;

		// Movement Vector
		float fAngle = Math::DegressToRadians(45.0f);
		BallDX = std::cosf(fAngle);
		BallDY = std::sinf(fAngle);

		ShowFps(true);

		return true;
	}

	virtual bool OnUserUpdate(sf::Time elapsed)
	{
		// Collision

		// Get current playerp position in string map cordinates
		int BallCellX = ball.getPosition().x / BlockSize.x;
		int BallCellY = ball.getPosition().y / BlockSize.y;

		// Get the position where the ball will be next and convert to std::wstring Map Coordinates
		int NewBallCellX = (ball.getPosition().x + BallDX) / BlockSize.x;
		int NewBallCellY = (ball.getPosition().y + BallDY) / BlockSize.y;

		if (map[(NewBallCellY * MapSizeX) + NewBallCellX] == L'#')
		{
			// Time da ce nova pozicija uvijek biti u drugoj celiji mozemo saznati s koje strane je bio collision
			// - Ako se sada nova pozicija nalazi u drugacijoj Y celiji nego sto se trenutno nalazi tada znamo da se collision dogodio na y osi pa se vektorov smjer na y osi mnozi sa -1.0f 
			if (NewBallCellX != BallCellX)
				BallDX *= -1;

			if (NewBallCellY != BallCellY)
				BallDY *= -1;

			std::cout << "X = " << BallDX << " Y = " << BallDY << std::endl;
		}

		// Move ball by it's movement vector
		ball.move(BallDX * elapsed.asSeconds() * fSpeed, BallDY * elapsed.asSeconds() * fSpeed);


		// Draw Map
		for (int y = 0; y < MapSizeY; y++)
		{
			for (int x = 0; x < MapSizeX; x++)
			{
				switch (map[(y * MapSizeX) + x])
				{
				case '#':
					block.setPosition(sf::Vector2f(x * BlockSize.x, y * BlockSize.y));
					Draw(block);

					break;
				}
			}
		}

		// Draw Ball
		Draw(ball);

		return true;
	}

};

int main()
{
	demo Test;
	Test.Construct(1280, 720, L"demo app");
	Test.Start();

	return 0;
}