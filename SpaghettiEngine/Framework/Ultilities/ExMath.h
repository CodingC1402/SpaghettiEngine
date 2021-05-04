#pragma once
#include <cmath>
#include <list>

typedef unsigned long long ULL;
typedef unsigned long long* PULL;
typedef const unsigned long long CULL;
typedef const unsigned long long* PCULL;

namespace CLib
{
	template<typename T>
	constexpr auto ToDouble(const T& value)
	{
		return static_cast<double>(value);
	}
	template<typename T>
	constexpr auto ToFloat(const T& value)
	{
		return static_cast<float>(value);
	}
	template<typename T>
	constexpr auto ToInt(const T& value)
	{
		return static_cast<int>(value);
	}
	template<typename T>
	constexpr auto ceili(const T& value)
	{
		return ToInt(std::ceil(value));
	}
		template<typename T>
	constexpr auto floori(const T& value)
	{
		return ToInt(std::floor(value));
	}

	template<typename T>
	constexpr T modulo(const T& f1, const T& f2)
	{
		return f1 - std::floor(f1 / f2) * f2;
	}
	template<typename T>
	constexpr bool compare(const T& f1, const T& f2, const T& epsilon)
	{
		if (fabs(f1 - f2) < epsilon)
			return true;
		return false;
	}
	template<typename T>
	constexpr bool compare(const T& f1, const T& f2)
	{
		if (fabs(f1 - f2) < 0.000001f)
			return true;
		return false;
	}
}