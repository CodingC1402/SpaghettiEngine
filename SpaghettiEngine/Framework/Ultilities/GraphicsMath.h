#pragma once
#include "CornDirectX.h"
#include <cmath>

/// <summary>
/// In the name of jusus the korean god, why do I have to do this,
/// why do I have to use directx9, it's so freaking janky >:((
/// </summary>

class GraphicsMath
{
public:
	static float	ToRad(const float& degree);
	static bool		CompareFloat(float x, float y, float epsilon = 0.000001f);
	static void		TransformVector3(Vector3& outVec, const Vector3& inVec, const Matrix& transformMatrix);
	static PMatrix	NewMatrix(); // Just WHY?, for god sake >:(
	static void		ZeroMatrix(Matrix* outMatrix);
	static void		RoundVector3(Vector3* vec);
	static void		TranslateMatrix(Matrix& matrix, const Vector3& vec);
	static void		MoveMatrix(Matrix& matrix, const Vector3& vec);
	static void		TranslateMatrix(Matrix& matrix, const float& x, const float& y, const float& z);
	static void		MoveMatrix(Matrix& matrix, const float& x, const float& y, const float& z);
	static void		Modulo(Vector3& out, const float& f);
	
	static Matrix	GetXAxisRotateMatrix(const float& degree);
	static Matrix	GetYAxisRotateMatrix(const float& degree);
	static Matrix	GetZAxisRotateMatrix(const float& degree);
	
	static bool		Inverse(const Matrix& matrix, Matrix& inverseMatrix);
	static void		GetCFactor(const Matrix& in, Matrix& out, int p, int q, int n);
	static float	GetDet(const Matrix& matrix, int n);
	static void 	GetAdj(const Matrix& in, Matrix& out);
public:
	static constexpr float pi = 3.141592f;
};
