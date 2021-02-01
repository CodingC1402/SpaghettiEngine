#pragma once
#include "WStrUtils.h"

class Debug
{
public:
	static void Log(std::string text);
	static void LogF( float number );
	static void LogB( bool flag );
};

