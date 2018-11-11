#include "Application.h"

int main()
{
	srand(time(0));

	Application MatrixRain(sf::Vector2u(1280, 720), "MatrixRain SFML");
	
	//For Matrix Rain only function
	MatrixRain.getMatrixRain()->MatrixSize(150, 12);

	while (!MatrixRain.getWindow()->isDone())
	{
		MatrixRain.HandleInput();
		MatrixRain.Update();
		MatrixRain.Render();
		
		MatrixRain.RestartClock();
	}

	return 0;
}