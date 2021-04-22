#pragma once
#include "CornDirectX.h"
#include <cmath>

/// <summary>
/// In the name of jusus the korean god, why do I have to do this,
/// why do I have to use directx9, it's so freaking janky >:((
/// </summary>

namespace exmath
{
	inline float modulo(const float& f1, const float& f2)
	{
		return f1 - std::floor(f1 / f2) * f2;
	}
}

class GraphicsMath
{
public:
	static float	ToRad(const float& degree);
	static bool		CompareFloat(float x, float y, float epsilon = 0.000001f);
	static void		TransformVector3(Vector3* outVec, const Vector3* inVec, PMatrix transformMatrix);
	static PMatrix	NewMatrix(); // Just WHY?, for god sake >:(
	static void		ZeroMatrix(Matrix* outMatrix);
	static void		RoundVector3(Vector3* vec);
	static void		TranslateMatrix(Matrix& matrix, const Vector3& vec);
	static void		MoveMatrix(Matrix& matrix, const Vector3& vec);
	static void		TranslateMatrix(Matrix& matrix, const float& x, const float& y, const float& z);
	static void		MoveMatrix(Matrix& matrix, const float& x, const float& y, const float& z);
	static void		Modulo(Vector3& out, const float& f);
public:
	static constexpr float pi = 3.141592f;
};
