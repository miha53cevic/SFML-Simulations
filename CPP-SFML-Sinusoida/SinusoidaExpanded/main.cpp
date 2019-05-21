#include <iostream>

#include "mihaSimpleSFML.h"
#include "util.h"

#define _USE_MATH_DEFINES
#include <math.h>

class SinusoidaExpanded : public mihaSimpleSFML
{
public:
	SinusoidaExpanded(float A, float B, float C, float D) 
	{
		this->A = A;
		this->B = B;
		this->C = C;
		this->D = D;
	}

private:
	sf::Vector2f offset;
	sf::Vector2i startPan;
	sf::Vector2f scale;

	lineStrip sinusoida;

	// Sinusoida variables
	float A, B, C, D;

	void ScreenToWorld(float screenX, float screenY, float& worldX, float& worldY)
	{
		worldX = screenX / scale.x + offset.x;
		worldY = screenY / scale.y + offset.y;
	}

	void WorldToScreen(float worldX, float worldY, float& screenX, float& screenY)
	{
		screenX = (worldX - offset.x) * scale.x;
		screenY = (worldY - offset.y) * scale.y;
	}

protected:
	void virtual Event(sf::Event e)
	{
		if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
		{
			startPan = sf::Mouse::getPosition(*getWindow());
		}
	}

	bool virtual OnUserCreate()
	{
		offset.x = 0.0f;
		offset.y = -ScreenHeight() / 2;

		scale.x = 1.0f;
		scale.y = 1.0f;

		return true;
	}

	bool virtual OnUserUpdate(sf::Time elapsed)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(*getWindow());

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			offset.x -= (mousePos.x - startPan.x) / scale.x;
			offset.y -= (mousePos.y - startPan.y) / scale.y;

			startPan.x = mousePos.x;
			startPan.y = mousePos.y;
		}

		sf::Vector2f beforeZoomMousePos;
		beforeZoomMousePos.x = mousePos.x;
		beforeZoomMousePos.y = mousePos.y;

		ScreenToWorld(mousePos.x, mousePos.y, beforeZoomMousePos.x, beforeZoomMousePos.y);

		// Change Scale
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{
			scale *= 0.99f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		{
			scale *= 1.01f;
		}

		sf::Vector2f afterZoomMousePos;
		afterZoomMousePos.x = mousePos.x;
		afterZoomMousePos.y = mousePos.y;

		ScreenToWorld(mousePos.x, mousePos.y, afterZoomMousePos.x, afterZoomMousePos.y);
		offset.x += (beforeZoomMousePos.x - afterZoomMousePos.x);
		offset.y += (beforeZoomMousePos.y - afterZoomMousePos.y);

		// Calculate sinusoida
		for (float x = 0; x < ScreenWidth(); x++)
		{
			// World coordinates
			float world_x = x;
			float world_y = A * sinf(B * (x + D) + C) + D;
			world_y *= -1;

			// Screen coordinates
			float sx, sy;
			WorldToScreen(world_x, world_y, sx, sy);

			sinusoida.getLineStrip()->append(sf::Vertex(sf::Vector2f(sx, sy), sf::Color::White));
		}

		// Stvori osi
		sf::Vector2f screenCoordX1(0,0);
		sf::Vector2f screenCoordX2(0,0);

		WorldToScreen(0, 0, screenCoordX1.x, screenCoordX1.y);
		WorldToScreen(ScreenWidth(), 0, screenCoordX2.x, screenCoordX2.y);

		sf::Vector2f screenCoordY1(0, 0);
		sf::Vector2f screenCoordY2(0, 0);

		WorldToScreen(ScreenWidth() / 2, -ScreenHeight() / 2, screenCoordY1.x, screenCoordY1.y);
		WorldToScreen(ScreenWidth() / 2, ScreenHeight() / 2, screenCoordY2.x, screenCoordY2.y);

		line osX(screenCoordX1, screenCoordX2, sf::Color::White);
		line osY(screenCoordY1, screenCoordY2, sf::Color::White);

		Draw(*osX.getLine());
		Draw(*osY.getLine());

		// Draw sinusoida
		Draw(*sinusoida.getLineStrip());

		// Clear last drawn
		sinusoida.getLineStrip()->clear();

		return true;
	}
};

int main(int argc, char* argv[])
{
	SinusoidaExpanded demo(100, 25, 0, 0);
	demo.Construct(1280, 720, L"SinusoidaExpanded");
	demo.Start();

	return 0;
}