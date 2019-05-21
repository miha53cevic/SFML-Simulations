/*////////////////////////////////////////////
    - @Mihael Petricevic
    - Date: 24.1.2019.
    - Inspiration: https://www.youtube.com/watch?v=p4Iz0XJY-Qk
    
    - Matrix Multiplication: http://matrixmultiplication.xyz/
    - Rotation Matrix: https://en.wikipedia.org/wiki/Rotation_matrix
    - 3D Projection: https://en.wikipedia.org/wiki/3D_projection
*/////////////////////////////////////////////

#include "headers/mihaSimpleSFML.hpp"

#include "headers/matrix.hpp"
#include "headers/vector3.hpp"

#include <math.h>

class demo : public mihaSimpleSFML
{
public:
    demo() {}

private:
    struct Point
    {
        Point()
        {
            rect.setSize(sf::Vector2f(5, 5));
            rect.setFillColor(sf::Color::White);
            rect.setOrigin(rect.getSize().x / 2, rect.getSize().y / 2);
        }

        void setPos(Vector3 x)
        {
            rect.setPosition(x.x, x.y);

            // Makni ovo i stavi da je angle += 0.0005f i dobijes istu stvar
            pos.x = x.x;
            pos.y = x.y;
            pos.z = x.z;
        }
        void setPos(float x = 0, float y = 0, float z = 0)
        {
            pos.x = x;
            pos.y = y;
            pos.z = z;

            rect.setPosition(pos.x, pos.y);
        }

        sf::RectangleShape rect;

        Vector3 pos;
    };

    std::vector<Point> vecPoints;
    
    sf::Transform trans;
    float angle;

    Matrix::Multiplication3Dto2D<float> Matrix;

protected:
    virtual bool OnUserCreate() override
    {
        Point a, b, c, d, e, f, g, h;
        a.setPos(-0.5, -0.5, -0.5);
        b.setPos(+0.5, -0.5, -0.5);
        c.setPos(+0.5, +0.5, -0.5);
        d.setPos(-0.5, +0.5, -0.5);
        e.setPos(-0.5, -0.5, +0.5);
        f.setPos(+0.5, -0.5, +0.5);
        g.setPos(+0.5, +0.5, +0.5);
        h.setPos(-0.5, +0.5, +0.5);

        vecPoints = {a, b, c, d, e, f, g, h};

        trans.translate(ScreenWidth() / 2, ScreenHeight() / 2);

        angle = 0;

        for (auto &i : vecPoints)
        {
            i.pos.mult(100);
            //i.rect.setFillColor(sf::Color::Green);
        }

        return true;
    }

    virtual bool OnUserUpdate(sf::Time elapsed) override
    {
        // XY Rotation Matrix
        std::vector<std::vector<float>> mxy = 
        {   { cosf(angle), -sinf(angle), 0 },
            { sinf(angle), +cosf(angle), 0 }    };
        
        // X Rotation Matrix
        std::vector<std::vector<float>> mx =
        {   { 1, 0, 0 },
            { 0, cosf(angle), -sinf(angle) },
            { 0, sinf(angle), +cosf(angle) }     };

        // Y Rotation Matrix
        std::vector<std::vector<float>> my =
        {   { +cosf(angle), 0, sinf(angle) },
            { 0, 1, 0 },
            { -sinf(angle), 0, cosf(angle) }     };

        // Z Rotation Matrix
        std::vector<std::vector<float>> mz =
        {   { cosf(angle), -sinf(angle), 0 },
            { sinf(angle), +cosf(angle), 0 },
            { 0, 0, 1 }     };

        // Draw Points
        for (auto &i : vecPoints)
        {
            Draw(i.rect, trans);
            
            Vector3 rotated = Matrix.mult(mx, 3, 3, i.pos);
            i.setPos(Matrix.mult(my, 3, 3, rotated));
            
            //std::cout << i.pos.x << " " << i.pos.y << " " << i.pos.z << "\n";
        }

        //angle += 0.0005f;
        angle = 0.0005f;

        auto connect = [&](int i, int j, auto vec)
        {
            sf::VertexArray line(sf::Lines, 2);
            line[0].position = sf::Vector2f(vec[i].rect.getPosition());
            line[1].position = sf::Vector2f(vec[j].rect.getPosition());

            Draw(line, trans);   
        };

        for (int i = 0; i < 4; i++) 
        {
            connect(i, (i + 1) % 4, vecPoints);
            connect(i + 4, ((i + 1) % 4) + 4, vecPoints);
            connect(i, i + 4, vecPoints);
        }

        return true;
    }
};

int main()
{
    demo app;
    app.Start();

    return 0;
}
