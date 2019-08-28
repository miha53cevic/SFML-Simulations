/*
    AUTHOR: miha53cevic
    DATE: 28.8.2019.
    Based on: javidx9's MODE7 video
    https://youtu.be/ybLZyY655iY
*/

#include "mihaSimpleSFML.h"

#include <math.h>

class Mode7 : public mihaSimpleSFML
{
public:
    Mode7() {}

private:
    sf::Image       m_mapImage;
    sf::Vector2i    m_nMapSize;

    float           m_fNear;
    float           m_fFar;

    sf::Vector2f    m_fWorld;
    float           m_fWorldAngle;

    float           m_fHalfFOV;

    sf::VertexArray m_pixels;
    unsigned int    m_pixelSize;

    bool            m_bWrapTexture;

private:
    void DrawPixel(int x, int y, sf::Color c)
    {
        int width = ScreenWidth() / m_pixelSize;

        int sx = x / m_pixelSize;
        int sy = y / m_pixelSize;

        int index = (sy * width) + sx;

        for (int i = 0; i < 4; i++)
            m_pixels[index * 4 + i].color = c;
    }

    sf::Color SamplePicture(float x, float y)
    {
        int sampleX = x * m_nMapSize.x;
        int sampleY = y * m_nMapSize.y;

        if (sampleX < 0 || sampleX >= m_nMapSize.x || sampleY < 0 || sampleY >= m_nMapSize.y)
        {
            return sf::Color::Black;
        }
        else return m_mapImage.getPixel(sampleX, sampleY);
    }

protected:
    void Event(sf::Event e) override
    {
    }

protected:
    bool OnUserCreate() override
    {
        m_fNear         = 0.01f;
        m_fFar          = 0.1f;

        m_fWorld        = { 0.0f, 0.0f };
        m_fWorldAngle   = 0.0f;

        m_fHalfFOV      = PI / 4;

        // Load Map
        if (!m_mapImage.loadFromFile("image.png"))
        {
            // Error map could not be loaded

            m_nMapSize = { 1024, 1024 };

            // Create image with checkerboard pattern
            m_mapImage.create(m_nMapSize.x, m_nMapSize.y, sf::Color::Black);

            for (int y = 0; y < m_nMapSize.y; y += 32)
            {
                for (int x = 0; x < m_nMapSize.x; x++)
                {
                    m_mapImage.setPixel(x, y,     sf::Color::White);
                    m_mapImage.setPixel(x, y + 1, sf::Color::White);

                    m_mapImage.setPixel(y, x,     sf::Color::Green);
                    m_mapImage.setPixel(y + 1, x, sf::Color::Green);
                }
            }
        }
        else
        {
            m_nMapSize.x = m_mapImage.getSize().x;
            m_nMapSize.y = m_mapImage.getSize().y;
        }

        // Create Pixels
        m_pixels.setPrimitiveType(sf::Quads);

        m_pixelSize = 4;

        for (int y = 0; y < ScreenHeight() / m_pixelSize; y++)
        {
            for (int x = 0; x < ScreenWidth() / m_pixelSize; x++)
            {
                float sx = x * m_pixelSize;
                float sy = y * m_pixelSize;

                m_pixels.append({ { sx,               sy               }, sf::Color::Black });
                m_pixels.append({ { sx + m_pixelSize, sy               }, sf::Color::Black });
                m_pixels.append({ { sx + m_pixelSize, sy + m_pixelSize }, sf::Color::Black });
                m_pixels.append({ { sx,               sy + m_pixelSize }, sf::Color::Black });
            }
        }

        // Wrap texture boolean
        m_bWrapTexture = true;

        EnableFPSCounter(true);

        return true;
    }

    bool OnUserUpdate(sf::Time elapsed) override
    {
        // Calculate Viewport near and far points & the view frustum
        sf::Vector2f fNear1, fFar1, fNear2, fFar2;

        /*
            Explanation
                x = Cx + cosf(angle) * Radius
                y = Cy + sinf(angle) * Radius
        */
        fNear1.x = m_fWorld.x + cosf(m_fWorldAngle - m_fHalfFOV) * m_fNear;
        fNear1.y = m_fWorld.y + sinf(m_fWorldAngle - m_fHalfFOV) * m_fNear;

        fNear2.x = m_fWorld.x + cosf(m_fWorldAngle + m_fHalfFOV) * m_fNear;
        fNear2.y = m_fWorld.y + sinf(m_fWorldAngle + m_fHalfFOV) * m_fNear;

        fFar1.x = m_fWorld.x + cosf(m_fWorldAngle - m_fHalfFOV) * m_fFar;
        fFar1.y = m_fWorld.y + sinf(m_fWorldAngle - m_fHalfFOV) * m_fFar;

        fFar2.x = m_fWorld.x + cosf(m_fWorldAngle + m_fHalfFOV) * m_fFar;
        fFar2.y = m_fWorld.y + sinf(m_fWorldAngle + m_fHalfFOV) * m_fFar;

        // Starting with furthest away line and work towards the camera point
        for (int y = 0; y < ScreenHeight() / 2; y++) // ScreenHeight / 2 jer hocemo samo doljni dio de se crta mapa, poslije se kod crtanja na y dodaje ScreenHeight / 2 pa je na dnu mapa
        {
            // Sampling Algorithm
            float fSampleDepth = (float)y / ((float)ScreenHeight() / 2.0f);

            // Line start and end points
            sf::Vector2f fStart, fEnd;

            fStart.x   = (fFar1.x - fNear1.x) / (fSampleDepth) + fNear1.x;
            fStart.y   = (fFar1.y - fNear1.y) / (fSampleDepth) + fNear1.y;
            fEnd.x     = (fFar2.x - fNear2.x) / (fSampleDepth) + fNear2.x;
            fEnd.y     = (fFar2.y - fNear2.y) / (fSampleDepth) + fNear2.y;

            for (int x = 0; x < ScreenWidth(); x++)
            {
                float fSampleWidth = (float)x / (float)ScreenWidth();

                // Sample points of the texutre / map to take
                sf::Vector2f fSample;

                fSample.x = (fEnd.x - fStart.x) * fSampleWidth + fStart.x;
                fSample.y = (fEnd.y - fStart.y) * fSampleWidth + fStart.y;

                // Wrap sample coordinates so it makes the map feel infinite
                // in other words make the texture repeat
                // fSamples are values between 0 and 1
                if (m_bWrapTexture)
                {
                    fSample.x = fmod(fSample.x, 1.0f);
                    fSample.y = fmod(fSample.y, 1.0f);
                }

                // Sample pixels relative to the picture
                sf::Color pixel = SamplePicture(fSample.x, fSample.y);

                DrawPixel(x, y + ScreenHeight() / 2, pixel);
            }
        }

        // Draw onto the screen
        Draw(m_pixels);

        // Change Variables
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) m_fFar += 0.1f * elapsed.asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) m_fFar -= 0.1f * elapsed.asSeconds();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) m_fNear += 0.1f * elapsed.asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) m_fNear -= 0.1f * elapsed.asSeconds();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) m_fHalfFOV += 0.1f * elapsed.asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) m_fHalfFOV -= 0.1f * elapsed.asSeconds();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  m_fWorldAngle -= 0.8f * elapsed.asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) m_fWorldAngle += 0.8f * elapsed.asSeconds();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            m_fWorld.x += cosf(m_fWorldAngle) * 0.8f * elapsed.asSeconds();
            m_fWorld.y += sinf(m_fWorldAngle) * 0.8f * elapsed.asSeconds();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            m_fWorld.x -= cosf(m_fWorldAngle) * 0.8f * elapsed.asSeconds();
            m_fWorld.y -= sinf(m_fWorldAngle) * 0.8f * elapsed.asSeconds();
        }

        return true;
    }

};

int main()
{
    Mode7 app;
    app.Construct(1280, 720, L"Mode7");
    app.Start();

    return 0;
}