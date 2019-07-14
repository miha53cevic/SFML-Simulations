/*
// Author: Mihael Petricevic
// Contact: miha53cevic@gmail.com
// Date: 14.7.2019.
//
// Sources: Wikipedia
//			https://en.wikipedia.org/wiki/Bowyer%E2%80%93Watson_algorithm
//
*/

#include "mihaSimpleSFML.h"

#include "Util.h"

class LowPolify : public mihaSimpleSFML
{
public:
	LowPolify() {}

private:

	void DrawPoint(sf::Vector2f pos, float size, sf::Color c = sf::Color::White)
	{
		sf::CircleShape circle;
		circle.setRadius(size);
		circle.setOrigin(size, size);
		circle.setFillColor(c);
		circle.setPosition(pos);

		Draw(circle);
	}

	void DrawEdge(Edge a)
	{
		sf::VertexArray line;
		line.setPrimitiveType(sf::Lines);
		line.append(sf::Vertex(sf::Vector2f(a.points[0].x, a.points[0].y), sf::Color::White));
		line.append(sf::Vertex(sf::Vector2f(a.points[1].x, a.points[1].y), sf::Color::White));

		Draw(line);
	}

	void DrawTriangle(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Color colour = sf::Color::White)
	{
		sf::VertexArray shape;
		shape.setPrimitiveType(sf::Triangles);
		shape.append(sf::Vertex(a, colour));
		shape.append(sf::Vertex(b, colour));
		shape.append(sf::Vertex(c, colour));

		Draw(shape);
	}

	Triangle startingTriangle;

	std::vector<sf::Vector2f> pointArray;

	std::vector<Triangle> vecTriangles;

protected:
	virtual bool OnUserCreate() override
	{
		int WIDTH = ScreenWidth();
		int HEIGHT = ScreenHeight();

		// creating super triangle
		sf::Vector2f a(0, HEIGHT);
		sf::Vector2f b(HEIGHT / tanf(toRadian(45)) + WIDTH, HEIGHT);
		sf::Vector2f c(0, tanf(toRadian(45)) * WIDTH * -1);

		startingTriangle.SetupOverPoints(a, b, c);
		
		while (pointArray.size() != 100)
		{
			float x = iRandom(0, WIDTH);
			float y = iRandom(0, HEIGHT);

			pointArray.push_back({ x, y });
		}

		// add super-triangle to triangulation
		vecTriangles.push_back(startingTriangle);

		// for each point in pointList do
		for (auto& point : pointArray)
		{
			// badTriangles := empty set
			std::vector<Triangle> badTriangles;

			// for each triangle in triangulation do
			for (auto& triangle : vecTriangles)
			{
				// if point is inside circumcircle of triangle
				triangle.isBad = triangle.isPointInCircumcircle(point);

				// add triangle to badTriangles
				if (triangle.isBad)
					badTriangles.push_back(triangle);
			}
			
			// polygon := empty set
			std::vector<Edge> Polygon;

			// for each triangle in badTriangles do
			for (auto& triangle : badTriangles)
			{
				// for each edge in triangle do
				for (int i = 0; i < 3; i++)
				{
					bool isShared = false;

					for (auto& triangle2 : badTriangles)
					{
						if (triangle == triangle2)
							continue;
						
						for (int j = 0; j < 3; j++)
						{
							// if edge is not shared by any other triangles in badTriangles
							if (triangle.getEdge(i) == triangle2.getEdge(j))
							{
								isShared = true;
							}
						}
						
					}

					// add edge to polygon
					if (!isShared)
					{
						Polygon.push_back(triangle.getEdge(i));
					}
				}
			}

			// for each triangle in badTriangles do
			for (auto iter = vecTriangles.begin(); iter != vecTriangles.end();)
			{
				// remove triangle from triangulation
				if (iter->isBad)
				{
					iter = vecTriangles.erase(iter);
				}
				else iter++;
			}

			// for each edge in polygon do
			for (auto &edge : Polygon)
			{
				// newTri := form a triangle from edge to point
				Triangle newTri(point, edge.points[0], edge.points[1]);

				// add newTri to triangulation
				vecTriangles.push_back(newTri);
			}
		}

		// for each triangle in triangulation
		for (auto iter = vecTriangles.begin(); iter != vecTriangles.end();)
		{
			// if triangle contains a vertex from original super - triangle
			bool containsVertex = false;

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					if (iter->getPoint(i) == startingTriangle.getPoint(j))
					{
						containsVertex = true;
					}
				}
			}

			// remove triangle from triangulation
			if (containsVertex)
			{
				iter = vecTriangles.erase(iter);
			}
			else iter++;
		}

		std::cout << vecTriangles.size() << std::endl;

		return true;
	}

	virtual bool OnUserUpdate(sf::Time elapsed) override
	{
		for (auto &i : pointArray)
		{
			DrawPoint(i, 5);
		}
		
		DrawEdge(startingTriangle.getEdge(0));
		DrawEdge(startingTriangle.getEdge(1));
		DrawEdge(startingTriangle.getEdge(2));
		

		for (auto &t : vecTriangles)
		{
			DrawTriangle(t.getPoint(0), t.getPoint(1), t.getPoint(2), sf::Color(iRandom(0, 255), iRandom(0, 255), iRandom(0, 255)));

			for (int i = 0; i < 3; i++)
			{
				DrawEdge(t.getEdge(i));
			}
		}

		return true;
	}
};

int main()
{
	LowPolify app;
	app.Construct(1280, 720, L"demo app");
	app.Start();

	return 0;
}