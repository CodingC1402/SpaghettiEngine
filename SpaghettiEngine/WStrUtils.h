#pragma once
#include <Windows.h>
#include <math.h>
#include <string>
#include <locale>
#include <codecvt>
#include <vector>

class WStrUtils
{
public:
	static PWSTR ToString( float number );
	static LPWSTR ConvertString( const std::string& instr );
	static LPWSTR Clone( LPCWSTR str );
};

