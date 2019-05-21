/*
*   Mihael Petričević
*   13.5.2019.
*   Source: https://en.wikipedia.org/wiki/Spirograph
*/

#include <iostream>
#include <algorithm>
#include <vector>

#include "mihaSimpleSFML.hpp"
#include "util.hpp"

class Spirograph : public mihaSimpleSFML
{
public:
    Spirograph(float R, float r, float p) 
    {
        this->R = R;
        this->r = r;
        this->p = p;
    }

private:
    std::vector<line> m_lines;
	std::vector<line> m_shape;

    // t = kut od 0 - 2PI
    float k, l, t;

    // Udaljenost od centra Ci
    float p;

    // Polumjer manje r i vece R kruznice
    float R, r;

protected:
    virtual bool OnUserCreate()
    {   
        // l = distance of A from center of Ci, k = how big is the inner circle Ci compared to the outer one Co
        k = r / R;
        l = p / r;
        
        std::cout << "l = " << l << " k = " << k << "\n";

		getWindow()->setFramerateLimit(300);

		float prevSX = (ScreenWidth() / 2) + R, prevSY = (ScreenHeight() / 2);

		// Draw Co - Outer circle
		for (float t = 0; t < M_PI * 2.0f; t += 0.01f)
		{
			float sx = (ScreenWidth() / 2) + (R * cosf(t));
			float sy = (ScreenHeight() / 2) + (R * sinf(t));

			line temp(prevSX, prevSY, sx, sy, sf::Color::Blue);
			m_lines.push_back(temp);

			prevSX = sx;
			prevSY = sy;
		}

        return true;
    }

    virtual bool OnUserUpdate(sf::Time elapsed)
    {
        float x = R * ((1 - k) * cosf(t) + l * k * cosf((1 - k) / k * t)) + ScreenWidth() / 2;
        float y = R * ((1 - k) * sinf(t) - l * k * sinf((1 - k) / k * t)) + ScreenHeight() / 2;

        t += 0.005f;
        
        // Nakon jednog kruga nemoj crtati vise jer je memory overflow
        if (t <= 6.0f * M_PI)
        {
			if (m_shape.empty())
			{
				line temp(x, y, x, y, sf::Color::White);
				m_shape.push_back(temp);
			}
			else
			{
				line temp(m_shape.at(m_shape.size() - 1).getLine()[1].position.x, m_shape.at(m_shape.size() - 1).getLine()[1].position.y, x, y, sf::Color::White);
				m_shape.push_back(temp);
			}
        }

        std::cout << m_shape.size() << "\n";

		// Draw shape made by Spirograph
		for (auto& i : m_shape)
		{
			Draw(i.getLine());
		}

		// Draw Co and Ci
		for (auto &i : m_lines)
		{
			Draw(i.getLine());
		}

        // Srediste ekrana i srediste kruznice Co
        sf::RectangleShape center;
        center.setSize(sf::Vector2f(4, 4));
        center.setOrigin(center.getSize().x / 2, center.getSize().y / 2);
        center.setFillColor(sf::Color::Green);
        center.setPosition(ScreenWidth() / 2, ScreenHeight() / 2);

        Draw(center);

        // Calculate the circle that Ci moves on
        float sx2 = (ScreenWidth() / 2) + ((R - r) * cosf(t));
        float sy2 = (ScreenHeight() / 2) + ((R - r) * sinf(t));

		float prevSX3 = sx2 + r;
		float prevSY3 = sy2;

        // Draw Ci which moves on (sx2, sy2) circle
        for (float i = 0.0f; i <= M_PI * 2.0f; i += 0.01f)
        {
            float sx3 = sx2 + (r * cosf(i));
            float sy3 = sy2 + (r * sinf(i));

            line a(prevSX3, prevSY3, sx3, sy3, sf::Color::Green);
            Draw(a.getLine());

			prevSX3 = sx3;
			prevSY3 = sy3;

            line temp(sx2, sy2, x, y, sf::Color::Magenta);
            Draw(temp.getLine());
        }

        return true;
    }
};

int main(int argc, char *argv[])
{
    float R = 200, r = 75, p = 30;

    if (argc > 1 && argc == 4)
    {
        R = std::stoi(argv[1]);
        r = std::stoi(argv[2]);
        p = std::stoi(argv[3]);
    }

    Spirograph demo(R, r, p);
    demo.Construct(640, 480, L"Spirograph SFML");
    demo.Start();

    return 0;
}