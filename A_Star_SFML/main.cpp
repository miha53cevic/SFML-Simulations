/*////////////////////////////////////////////
	- @Mihael Petricevic
	- Date: 30.3.2019.
	- https://en.wikipedia.org/wiki/A*_search_algorithm
*/////////////////////////////////////////////

#include "mihaSimpleSFML.hpp"

#include <math.h> // Contains INFINITY
#include <vector>
#include <algorithm>
#include <iostream>

struct Node 
{
    float gCost = INFINITY;
    float fCost = INFINITY;

    std::vector<Node*> neighbours;

    Node* parent = nullptr;

    bool bObstacle = false;
    bool bStartingNode = false;
    bool bEndingNode = false;
	bool bEvaluated = false;

    int x, y;
};

enum NodeType
{
	START, GOAL, OBSTACLE
};

class AStar : public mihaSimpleSFML 
{
public:
    AStar() {}

private:
    sf::RectangleShape rect;
    std::vector<Node> nodes;

    int blockSize = 32;

    std::vector<Node*> path;

	NodeType selection = START;

	bool bsetStart = false;
	bool bsetGoal = false;

	Node* start;
	Node* goal;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::vector<Node*> reconstruct_path(Node* current)
    {
        std::vector<Node*> totalPath;
        totalPath.push_back(current);

        Node* temp = current;

        while (temp->parent != nullptr)
        {
            temp = temp->parent;
            totalPath.push_back(temp);
        }
		
        return totalPath;
    }

    std::vector<Node*> Execute_AStar(Node* start, Node* goal)
    {
        // Nodes to evaluate, na pocetku je to samo start node
        std::vector<Node*> openSet;
        openSet.push_back(start);

        // Nodes already evaluated
        std::vector<Node*> closedSet;

        // Cost from going from starting node to starting node is 0
        start->gCost = 0;

        // FCost na start node-u je samo distance jer je gCost = 0
        start->fCost = distance(start, goal);

        while (openSet.size() != 0)
        {
            // Get node with lowest fCost
            std::sort(openSet.begin(), openSet.end(), [](Node* a, Node* b) { return a->fCost < b->fCost; });
            Node* current = openSet.at(0);

            if (current == goal) 
                return reconstruct_path(current);

            for (std::vector<Node*>::iterator itr = openSet.begin(); itr != openSet.end(); itr++)
            {
                if (*itr == current)
                {
                    openSet.erase(itr);
                    break;
                }
            }
    
            closedSet.push_back(current);

            int x = current->x / blockSize;
            int y = current->y / blockSize;

            // Get Neighbours for current
            if (x != 0) 
                current->neighbours.push_back(&nodes.at((y * (ScreenWidth() / blockSize) + (x - 1))));

            if (x != ScreenWidth() / blockSize - 1)
                current->neighbours.push_back(&nodes.at((y * (ScreenWidth() / blockSize) + (x + 1))));
            
            if (y != 0)
                current->neighbours.push_back(&nodes.at(((y - 1) * (ScreenWidth() / blockSize) + x)));

            if (y != ScreenHeight() / blockSize - 1)
                current->neighbours.push_back(&nodes.at(((y + 1) * (ScreenWidth() / blockSize) + x)));
            
            for (auto &neighbour : current->neighbours)
            {
                // If node is already in closedSet skip, std::find ako ne nade vraca last
                if (std::find(closedSet.begin(), closedSet.end(), neighbour) != closedSet.end())
                    continue;

				// If node is a obstacle skip it
				if (neighbour->bObstacle)
					continue;
                
                float neighbour_gCost = current->gCost + distance(current, neighbour);

                if (std::find(openSet.begin(), openSet.end(), neighbour) == openSet.end())
					openSet.push_back(neighbour);
                else if (neighbour_gCost >= neighbour->gCost)
                    continue;

                neighbour->parent = current;
                neighbour->gCost = neighbour_gCost;
                neighbour->fCost = neighbour->gCost + distance(neighbour, goal);
            }
        }

		// If no path found return Nullptr
		return { nullptr };
    }

	// Get distance between 2 dots
    float distance(Node* start, Node* goal) { return sqrtf(powf(goal->x - start->x, 2) + powf(goal->y - start->y, 2)); }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Update after changing start and goal
	void update()
	{
		// Execute A* Algorithm
		path = Execute_AStar(start, goal);

		// Remove the start and goal nodes from the path so you can see them better
		path.erase(path.begin());
		path.erase(path.end() - 1);
	}

	void reset()
	{
		// Reset nodes to default values
		for (auto& i : nodes)
		{
			i.gCost = INFINITY;
			i.fCost = INFINITY;
			i.neighbours.clear();
			i.parent = nullptr;
		}
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

protected:
	virtual void Event(sf::Event e)
	{
		if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::S) selection = START;
		else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::E) selection = GOAL;
		else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::O) selection = OBSTACLE;

		// Remove all obstacles
		if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Space)
		{
			for (auto& i : nodes)
			{
				i.bObstacle = false;
			}
		}

		// Mouse Events
		if (e.type == e.MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left)
		{
			reset();

			// Get node position from mouse click position
			sf::Vector2i mousePos = sf::Mouse::getPosition(*getWindow());

			int x = mousePos.x / blockSize;
			int y = mousePos.y / blockSize;

			// Set start and goal node
			if (selection == NodeType::START)
			{
				// Remove old starting node
				start->bStartingNode = false;

				start = &nodes.at((y * (ScreenWidth() / blockSize) + x));
				start->bStartingNode = true;
			}
			else if (selection == NodeType::GOAL)
			{
				// Remove old goal node
				goal->bEndingNode = false;

				goal = &nodes.at((y * (ScreenWidth() / blockSize) + x));;
				goal->bEndingNode = true;
			}
			else if (selection == NodeType::OBSTACLE)
			{
				nodes.at((y * (ScreenWidth() / blockSize) + x)).bObstacle = true;
			}

			// Update after changing start and goal have been changed or a obstacle has been added
				update();
		}
	}

    bool virtual OnUserCreate()
    {
        rect.setSize(sf::Vector2f(blockSize * 0.9f, blockSize * 0.9f));

        for (int y= 0; y < (int)(ScreenHeight() / blockSize); y++)
        {
            for (int x = 0; x < (int)(ScreenWidth() / blockSize); x++)
            {
                Node temp;
                temp.x = x * blockSize;
                temp.y = y * blockSize;
                nodes.push_back(temp);
            }
        }

		// Initial start and goal positions
		start = &nodes.at((0 * (ScreenWidth() / blockSize) + 0));
		start->bStartingNode = true;

		goal = &nodes.at((5 * (ScreenWidth() / blockSize) + 5));
		goal->bEndingNode = true;

		// Run algorithm
		update();

        return true;
    }

    bool virtual OnUserUpdate(sf::Time elapsed)
    {
		// Draw nodes
        for (auto &i : nodes)
        {
            rect.setPosition(i.x, i.y);

            if (i.bStartingNode)
            {
                rect.setFillColor(sf::Color::Green);
            }
            else if (i.bEndingNode)
            {
                rect.setFillColor(sf::Color::Red);
            }
			else if (i.bObstacle)
			{
				rect.setFillColor(sf::Color(125, 125, 125));
			}
			else if (i.bEvaluated)
			{
				rect.setFillColor(sf::Color(0, 0, 255, 255));
			}
			else rect.setFillColor(sf::Color(0, 0, 255, 100));

            Draw(rect);
        }

		// Draw Path nodes
		for (auto &i : path)
		{
			rect.setPosition(i->x, i->y);
			rect.setFillColor(sf::Color::Yellow);

			Draw(rect);
		}

		return true;
    }
};

int main()
{
    AStar demo;
    demo.Construct(640, 480, L"A*");
    demo.Start();

    return 0;
}