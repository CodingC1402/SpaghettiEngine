#pragma once
#include <string>
#include <iostream>

class Debug
{
public:
	static void Log(const wchar_t* text);
	static void LogB( bool flag );

	template<typename T>
	static inline void LogNumber(const T& number)
	{
		std::wostringstream oss;
		oss << number << std::endl;
		Log(oss.str().c_str());
	}
};