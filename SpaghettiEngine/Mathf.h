#pragma once
#include <math.h>

class Mathf
{
public:
	static float Clamp( float value, float max, float min );
	static float RoundToPrecision( float number, int precision );
};

