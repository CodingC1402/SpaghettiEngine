#pragma once
#include <Windows.h>
#include <math.h>
#include <string>
#include <locale>
#include <codecvt>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Utils.h"

class WStrUtils
{
public:
	static PWSTR ToString( float number );
	static LPWSTR ConvertString( const std::string& instr );
	static LPWSTR Clone( LPCWSTR str );
	static bool ConvertString( std::string &s, const LPWSTR pw, UINT codepage );
};

