#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

class Math
{
public:
	Math() = default;

	static double PI()
	{
		return M_PI;
	}

	template<class T>
	static T DegressToRadians(T l_degress)
	{
		return l_degress * PI() / 180;
	}

private:
	
};