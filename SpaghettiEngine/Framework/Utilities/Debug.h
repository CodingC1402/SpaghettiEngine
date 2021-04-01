#pragma once
#include <string>

class Debug
{
public:
	static void Log(const wchar_t* text);
	static void LogF( float number );
	static void LogB( bool flag );
};

