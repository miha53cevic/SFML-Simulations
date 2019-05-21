/*////////////////////////////////////////////

    - @Mihael Petricevic
    - Date: 26.1.2019.
    - Source: https://web.archive.org/web/20160418004149/http://freespace.virgin.net/hugo.elias/graphics/x_water.htm
    - Inspiration: The Coding Train Water Ripple

*/////////////////////////////////////////////

#include "headers/mihaSimpleSFML.hpp"
#include <vector>

class demo : public mihaSimpleSFML
{
public:
    demo() {}

private:
    struct Pixel
    {
        Pixel()
            : m_pixel(sf::Points, 1)
            , col(0)
            {
                m_pixel[0].color = sf::Color(col, col, col);
            }

        void update()
        {
            m_pixel[0].color = sf::Color(col, col, col);
        }

        sf::VertexArray m_pixel;
        float col;
    };

    std::vector<Pixel> buffer1;
    std::vector<Pixel> buffer2;

    float damping;
    
protected:
    virtual bool OnUserCreate() override
    {
        // Fill buffer1 and buffer2
        for (int y = 0; y < ScreenHeight(); y++)
        {
            for (int x = 0; x < ScreenWidth(); x++)
            {
                Pixel newPixel;
                newPixel.m_pixel[0].position = sf::Vector2f(x, y);

                buffer1.push_back(newPixel);
                buffer2.push_back(newPixel);
            }
        }

        // Set Damping Factor
        damping = 0.9f;

        return true;
    }

    virtual bool OnUserUpdate(sf::Time elapsed) override
    {
        // Lamda Function Converts 2D to 1D COORDs
        auto index = [&](int x, int y) -> int
        {
            return (y * ScreenWidth()) + x;
        };

        // Make ripple at mouse pos and on mouse click
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(*getWindow());

            buffer1[index(mousePos.x, mousePos.y)].col = 255;
        }

        // Algorithm for rippling effect
        for (int y = 1; y < ScreenHeight() - 1; y++)
        {
            for (int x = 1; x < ScreenWidth() - 1; x++)
            {
                buffer2[index(x, y)].col = 
                    (   buffer1[index(x - 1, y + 0)].col +
                        buffer1[index(x + 1, y + 0)].col +
                        buffer1[index(x + 0, y + 1)].col +
                        buffer1[index(x + 0, y - 1)].col ) / 2 - buffer2[index(x,y)].col;

                buffer2[index(x,y)].col = buffer2[index(x,y)].col * damping;
            }
        }

        // Display Buffer2
        for (auto &i : buffer2)
        {
            i.update();

            // Draw only those who have their value changed from Default = 0
            if (i.col != 0)
            {
                Draw(i.m_pixel);
            }
        }

        // Swap the buffers 
        buffer1.swap(buffer2);

        return true;
    }
};

int main()
{
    const int WIDTH  = 200;
    const int HEIGHT = 200;

    demo app;
    app.Construct(WIDTH, HEIGHT, L"Water Ripple SFML");
    app.Start();

    return 0;
}