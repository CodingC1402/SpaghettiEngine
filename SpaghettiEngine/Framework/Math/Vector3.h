#pragma once
#include "Macros.h"
#include "Matrix.h"

class Vector3
{
public:
	Vector3() = default;
	Vector3(const Vector3& vec) = default;
	Vector3(float x, float y, float z);

	const float& operator[](unsigned index) const;
	float& operator[](unsigned index);

	bool operator== (const Vector3& vec) const;
	bool operator!= (const Vector3& vec) const;

	Vector3& operator= (const Vector3& vec);
	Vector3  operator+ (const Vector3& vec) const;
	Vector3  operator- (const Vector3& vec) const;
	Vector3  operator* (const float &scalar) const;
	Vector3  operator/ (const float &scalar) const;

	Vector3  operator* (const Matrix4& matrix) const;

	Vector3& operator+= (const Vector3& vec);
	Vector3& operator-= (const Vector3& vec);
	Vector3& operator*= (const float& scalar);
	Vector3& operator/= (const float& scalar);

	Vector3  Add(const Vector3& vec) const;
	Vector3  Subtract(const Vector3& vec) const;
	Vector3  GetUnitVector() const;
	Vector3  Div(const float& scalar) const;
	Vector3  Mul(const float& scalar) const;
	Vector3  Mul(const Matrix4& matrix) const;
	Vector3  Round();
	float    Dot(const Vector3& vec) const;
	float    GetMagnitude() const;
	float    GetPow2Magnitude() const;

	D3DXVECTOR3 ConvertToDxVector() const;
public:
	PROPERTY(float, x);
	GET(x) const { return _numbers[0]; }
	SET(x) { _numbers[0] = value; }

	PROPERTY(float, y);
	GET(y) const { return _numbers[1]; }
	SET(y) { _numbers[1] = value; }

	PROPERTY(float, z);
	GET(z) const { return _numbers[2]; }
	SET(z) { _numbers[2] = value; }
protected:
	float _numbers[3] = { 0 };
};