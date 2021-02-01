#pragma once
#include <Windows.h>
#include <math.h>
#include <string>

class WStrUtils
{
public:
	static PWSTR ToString( float number );
	static LPWSTR ConvertString( const std::string& instr );
};

