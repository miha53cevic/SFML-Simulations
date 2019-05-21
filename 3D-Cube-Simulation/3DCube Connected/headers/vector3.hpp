#pragma once

class Vector3
{
public:
    Vector3()
    {
        this->x = 0;
        this->y = 0;
        this->z = 0; 
    }
    Vector3(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    
    void mult(float x)
    {
        this->x *= x;
        this->y *= x;
        this->z *= x;
    }

    void add(float x)
    {
        if (!first)
        {
            first = true;
            this->x = x;
        }
        else if (!second)
        {
            second = true;
            this->y = x;
        }
        else if (!third)
        {
            third = true;
            this->z = x;
        }
    }

    float x;
    float y;
    float z;

private:
    bool first  = false;
    bool second = false;
    bool third  = false;
};