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
#include "Trigonometrija.h"

#include <iostream>

class LowPolify : public mihaSimpleSFML
{
public:
	LowPolify() {}

private:

	void DrawPoint(vec2f pos, float size, sf::Color c = sf::Color::White)
	{
		sf::CircleShape circle;
		circle.setRadius(size);
		circle.setOrigin(size, size);
		circle.setFillColor(c);
		circle.setPosition(pos.x, pos.y);

		Draw(circle);
	}

	void DrawEdge(Edge a)
	{
		sf::VertexArray line;
		line.setPrimitiveType(sf::Lines);
		line.append(sf::Vertex(sf::Vector2f(a.a.x, a.a.y), sf::Color::White));
		line.append(sf::Vertex(sf::Vector2f(a.b.x, a.b.y), sf::Color::White));

		Draw(line);
	}

	sf::VertexArray CreateTriangleVertexArray(vec2f a, vec2f b, vec2f c, sf::Color colour = sf::Color::White)
	{
		sf::VertexArray shape;
		shape.setPrimitiveType(sf::Triangles);
        shape.append(sf::Vertex({ a.x, a.y }, colour));
		shape.append(sf::Vertex({ b.x, b.y }, colour));
		shape.append(sf::Vertex({ c.x, c.y }, colour));

		//Draw(shape);

		return shape;
	}

	Triangle startingTriangle;

	std::vector<vec2f> pointArray;

	std::vector<Triangle> vecTriangles;
	std::vector<sf::VertexArray> m_coloured_triangles;

protected:
	virtual bool OnUserCreate() override
	{
		int WIDTH = ScreenWidth();
		int HEIGHT = ScreenHeight();

		// creating super triangle
		vec2f a(0, HEIGHT);
		vec2f b(HEIGHT / tanf(toRadian(45)) + WIDTH, HEIGHT);
		vec2f c(0, tanf(toRadian(45)) * WIDTH * -1);

		startingTriangle.Initialize(a, b, c);
       
		while (pointArray.size() != 100)
		{
			float x = fRandom(0, WIDTH);
			float y = fRandom(0, HEIGHT);
        
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
							if (triangle.edges[i] == triangle2.edges[j])
							{
								isShared = true;
							}
						}
						
					}

					// add edge to polygon
					if (!isShared)
					{
						Polygon.push_back(triangle.edges[i]);
					}
				}
			}

            // Erase bad triangles
            vecTriangles.erase(std::remove_if(vecTriangles.begin(), vecTriangles.end(), [](Triangle t) {return t.isBad; }), vecTriangles.end());

			// for each edge in polygon do
			for (auto &edge : Polygon)
			{
				// newTri := form a triangle from edge to point
				Triangle newTri(edge.a, edge.b, point);

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
					if (iter->points[i] == startingTriangle.points[j])
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

		// Colour in the triangles
		for (auto &t : vecTriangles)
		{
			m_coloured_triangles.push_back(CreateTriangleVertexArray(t.points[0], t.points[1], t.points[2], sf::Color(iRandom(0, 255), iRandom(0, 255), iRandom(0, 255))));
		}

		return true;
	}

	virtual bool OnUserUpdate(sf::Time elapsed) override
	{
		DrawEdge(startingTriangle.edges[0]);
		DrawEdge(startingTriangle.edges[1]);
		DrawEdge(startingTriangle.edges[2]);
		

		for (auto &t : vecTriangles)
		{
			//for (int i = 0; i < 3; i++)
			//{
			//	DrawEdge(t.edges[i]);
			//}

			for (auto &i : m_coloured_triangles)
			{
				Draw(i);
			}
		}

        for (auto &i : pointArray)
        {
            DrawPoint(i, 5);
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