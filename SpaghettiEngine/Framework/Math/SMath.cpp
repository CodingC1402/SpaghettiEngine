#include "SMath.h"

float SMath::Lerp(const float& start, const float& end, const float t)
{
	return start + (end - start) * ((t > 1.0f ? 1.0f : t) < 0.0f ? 0.0f : t);
}

float SMath::ToRad(const float& degree)
{
	return degree * (pi / 180.0f);
}

bool SMath::CompareFloat(float x, float y, float epsilon)
{
	if (fabs(x - y) < epsilon)
		return true;
	return false;
}

void SMath::TransformVector3(Vector3& outVec, Vector3& inVec, Matrix4& transformMatrix)
{
	outVec = inVec * transformMatrix;
}


void SMath::RoundVector3(Vector3* vec)
{
	vec->x = std::round(vec->x);
	vec->y = std::round(vec->y);
	vec->z = std::round(vec->z);
}

void SMath::TranslateMatrix(Matrix4& matrix, const Vector3& vec)
{
	matrix._41 += vec.x;
	matrix._42 += vec.y;
	matrix._43 += vec.z;
}

void SMath::MoveMatrix(Matrix4& matrix, const Vector3& vec)
{
	matrix._41 = vec.x;
	matrix._42 = vec.y;
	matrix._43 = vec.z;
}

void SMath::TranslateMatrix(Matrix4& matrix, const float& x, const float& y, const float& z)
{
	matrix._41 += x;
	matrix._42 += y;
	matrix._43 += z;
}

void SMath::MoveMatrix(Matrix4& matrix, const float& x, const float& y, const float& z)
{
	matrix._41 = x;
	matrix._42 = y;
	matrix._43 = z;
}

void SMath::Modulo(Vector3& out, const float& f)
{
	out.x = SMath::modulo(out.x, f);
	out.y = SMath::modulo(out.y, f);
	out.z = SMath::modulo(out.z, f);
}

Matrix4 SMath::GetXAxisRotateMatrix(const float& degree)
{
	Matrix4 XAxis;
	XAxis._11 = 1;
	XAxis._44 = 1;
	const float rad = SMath::ToRad(degree);
	XAxis._22 = std::cosf(rad);
	XAxis._23 = std::sinf(rad);
	XAxis._32 = -std::sinf(rad);
	XAxis._33 = std::cosf(rad);
	return XAxis;
}

Matrix4 SMath::GetYAxisRotateMatrix(const float& degree)
{
	Matrix4 YAxis;
	YAxis._22 = 1;
	YAxis._44 = 1;
	const float rad = SMath::ToRad(degree);
	YAxis._11 = std::cosf(rad);
	YAxis._13 = -std::sinf(rad);
	YAxis._31 = std::sinf(rad);
	YAxis._33 = std::cosf(rad);
	return YAxis;
}

Matrix4 SMath::GetZAxisRotateMatrix(const float& degree)
{
	Matrix4 ZAxis;
	ZAxis._33 = 1;
	ZAxis._44 = 1;
	const float rad = SMath::ToRad(degree);
	ZAxis._11 = std::cosf(rad);
	ZAxis._12 = std::sinf(rad);
	ZAxis._21 = -std::sinf(rad);
	ZAxis._22 = std::cosf(rad);
	return ZAxis;
}