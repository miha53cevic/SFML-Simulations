/*////////////////////////////////////////////
    - @Mihael Petricevic
    - Date: 22.1.2019.
    - https://en.wikipedia.org/wiki/Toothpick_sequence
*/////////////////////////////////////////////

#include "mihaSimpleSFML.hpp"

#include <vector>
#include <iostream>

//#define Debugging

class demo : public mihaSimpleSFML
{
private:
    struct Toothpick 
    {
        Toothpick(int dir)
            : line(sf::PrimitiveType::Lines, 2)
            , Length(99)
            , wasExpanded(false)
            , Direction(dir)
        {}

        void setLine(int x1, int y1, int x2, int y2)
        {
            line[0].position = sf::Vector2f(x1, y1);
            line[1].position = sf::Vector2f(x2, y2);
        }

        std::vector<Toothpick> Expand(std::vector<Toothpick> picks, std::vector<Toothpick>::iterator curPick)
        {
            // Pick has been expanded
            wasExpanded = true;

            // Get next pick Direction 
            int dir;

            if (this->Direction == 0)
            {
                dir = 1;
            }
            else dir = 0;

            Toothpick pick1(dir);
            Toothpick pick2(dir);

            // Vertical
            if (dir == 0)
            {
                pick1.setLine(this->line[0].position.x, this->line[0].position.y - pick1.Length / 2, this->line[0].position.x, this->line[0].position.y + pick1.Length / 2);
                pick2.setLine(this->line[1].position.x, this->line[1].position.y - pick2.Length / 2, this->line[1].position.x, this->line[1].position.y + pick2.Length / 2);
            }
            
            // Horizontal
            else
            {
                pick1.setLine(this->line[0].position.x - pick1.Length / 2, this->line[0].position.y, this->line[0].position.x + pick1.Length / 2, this->line[0].position.y);
                pick2.setLine(this->line[1].position.x - pick2.Length / 2, this->line[1].position.y, this->line[1].position.x + pick2.Length / 2, this->line[1].position.y);
            }

            // Check if Toothpick side is already used
            bool PushPick1 = true;
            bool PushPick2 = true;

            for (auto itr = picks.begin(); itr != picks.end(); itr++)
            {
                if (itr->line[0].position != curPick->line[0].position && itr->line[1].position != curPick->line[1].position)
                {
                    if (itr->line[0].position == this->line[0].position || itr->line[0].position == this->line[1].position)
                    {
                        PushPick2 = false;
                    }

                    if (itr->line[1].position == this->line[0].position || itr->line[1].position == this->line[1].position)
                    {
                        PushPick1 = false;
                    }
                }
            }

            std::vector<Toothpick> newLines;

            if (PushPick1) newLines.push_back(pick1);
            if (PushPick2) newLines.push_back(pick2);

            return newLines;
        }

        // Line object made out of 2 vertex points
        sf::VertexArray line;

        // Size of line
        int Length;

        // Has line been expanded
        bool wasExpanded;

        // 0 Vertical, 1 Horizontal
        int Direction;
    };

    std::vector<Toothpick> vecToothpicks;
    sf::View view = getWindow()->getView();
    
protected:
    virtual bool OnUserCreate()
    {   
        // Set view center
        view.setSize(ScreenWidth(), ScreenHeight());
        view.setCenter(320, 240);

        // Starting toothpick
        Toothpick pick(0);
        pick.setLine(ScreenWidth() / 2, ScreenHeight() / 2 - pick.Length / 2, ScreenWidth() / 2, ScreenHeight() / 2 + pick.Length / 2);

        vecToothpicks.push_back(pick);

        #ifdef Debbugging
        // Debugging
        for (auto &i : vecToothpicks)
        {
            std::cout << "X1 = " << i.line[0].position.x << " Y1 = " << i.line[0].position.y << std::endl;
            std::cout << "X2 = " << i.line[1].position.x << " Y2 = " << i.line[1].position.y << "\n\n\n\n";
        }
        #endif

        return true;
    }

    virtual void Event(sf::Event event)
    {
        // Press Space to advance generation
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
        {
            // Vector where newer toothpicks are sent after expend is called
            std::vector<Toothpick> vecNextPicks;

            for (auto itr = vecToothpicks.begin(); itr != vecToothpicks.end(); itr++)
            {
                if (!itr->wasExpanded)
                {
                    for (auto &j : itr->Expand(vecToothpicks, itr))
                    {
                        vecNextPicks.push_back(j);
                    }
                }
            }

            // Add new picks to all picks vector
            for (auto &i : vecNextPicks)
            {
                vecToothpicks.push_back(i);
            }
        }

        // If out of screen zoom out
        if (vecToothpicks.back().line[0].position.x >= view.getSize().x)
        {
            view.zoom(5);
            getWindow()->setView(view);
        }
    }

    virtual bool OnUserUpdate(sf::Time elapsed)
    {
        // Draw Lines
        for (auto &i : vecToothpicks)
        {
            Draw(i.line);
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
