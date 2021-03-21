#pragma once
#include <math.h>

class Mathd
{
public:
	static double Clamp( double value, double max, double min );
	static double RoundToPrecision( double number, int precision );
};

