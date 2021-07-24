#pragma once
#include "Vector3.h"
#include <cmath>

typedef unsigned long long ULL;
typedef unsigned long long* PULL;
typedef const unsigned long long CULL;
typedef const unsigned long long* PCULL;

typedef unsigned char UnsignedTiny;
typedef char Tiny;

Tiny operator""_t (unsigned long long value);
UnsignedTiny operator""_ut (unsigned long long value);

short operator""_s (unsigned long long value);
unsigned short operator""_us(unsigned long long value);

class SMath
{
public:
	static float	ToRad(const float& degree);
	static bool		CompareFloat(float x, float y, float epsilon = 0.000001f);
	static void		TransformVector3(Vector3& outVec, Vector3& inVec, Matrix4& transformMatrix);
	static void		RoundVector3(Vector3* vec);
	static void		TranslateMatrix(Matrix4& matrix, const Vector3& vec);
	static void		MoveMatrix(Matrix4& matrix, const Vector3& vec);
	static void		TranslateMatrix(Matrix4& matrix, const float& x, const float& y, const float& z);
	static void		MoveMatrix(Matrix4& matrix, const float& x, const float& y, const float& z);
	static void		Modulo(Vector3& out, const float& f);
	static Matrix4	GetXAxisRotateMatrix(const float& degree);
	static Matrix4	GetYAxisRotateMatrix(const float& degree);
	static Matrix4	GetZAxisRotateMatrix(const float& degree);

	template<typename T>
	static T Abs(const T& value)
	{
		if (value < 0)
			return value * -1;
		else
			return value;
	}
	template<typename T>
	static T Lerp(const T& start, const T& end, const float& t)
	{
		return start + (end - start) * ((t > 1.0f ? 1.0f : t) < 0.0f ? 0.0f : t);
	}
	template<typename T1, typename T2>
	static constexpr auto Max(const T1& value1, const T2& value2)
	{
		return value1 > value2 ? value1 : value2;
	}
	template<typename T1, typename T2>
	static constexpr auto Min(const T1& value1, const T2& value2)
	{
		return value1 < value2 ? value1 : value2;
	}
	template<typename T>
	static constexpr auto Tofloat(const T& value)
	{
		return static_cast<float>(value);
	}
	template<typename T>
	static constexpr auto ToFloat(const T& value)
	{
		return static_cast<float>(value);
	}
	template<typename T>
	static constexpr auto ToInt(const T& value)
	{
		return static_cast<int>(value);
	}
	template<typename T>
	static constexpr auto ceili(const T& value)
	{
		return ToInt(std::ceil(value));
	}
	template<typename T>
	static constexpr auto floori(const T& value)
	{
		return ToInt(std::floor(value));
	}

	template<typename T>
	static constexpr T modulo(const T& f1, const T& f2)
	{
		return static_cast<T>(f1 - std::floor(f1 / f2) * f2);
	}
	template<typename T>
	static constexpr bool compare(const T& f1, const T& f2, const T& epsilon)
	{
		if (fabs(f1 - f2) < epsilon)
			return true;
		return false;
	}
	template<typename T>
	static constexpr bool compare(const T& f1, const T& f2)
	{
		if (fabs(f1 - f2) < 0.000001f)
			return true;
		return false;
	}
public:
	static constexpr float pi = 3.141592f;
};