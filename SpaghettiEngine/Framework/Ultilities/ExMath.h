#pragma once
#include <cmath>

namespace ExMath
{
	template<typename T>
	constexpr auto ToDouble(const T& value) { return static_cast<double>(value); }
	template<typename T>
	constexpr auto ToFloat(const T& value) { return static_cast<float>(value); }
	template<typename T>
	constexpr auto ToInt(const T& value) { return static_cast<int>(value); }
	template<typename T>
	constexpr auto ceili(const T& value) { return ToInt(std::ceil(value)); }
}