#pragma once
#include "CornDirectX.h"
#include "Macros.h"

class Matrix4
{
public:
	Matrix4(const float numbers[]);
	Matrix4() = default;

	const float* operator[] (unsigned row) const;
	float* operator[] (unsigned row);

	static Matrix4 GetDiagonalMatrix();

	Matrix4& operator= (const Matrix4& matrix);
	Matrix4& operator*=(const Matrix4& matrix);
	Matrix4& operator/=(const Matrix4& matrix);
	Matrix4  operator* (const Matrix4& matrix) const;
	Matrix4  operator/ (const Matrix4& matrix) const;
	Matrix4  operator+ (const Matrix4& matrix) const;
	Matrix4  operator- (const Matrix4& matrix) const;

	Matrix4& operator*= (const float& scalar);
	Matrix4& operator/= (const float& scalar);
	Matrix4  operator*  (const float& scalar) const;
	Matrix4  operator/  (const float& scalar) const;

	Matrix4& SelfMul(const Matrix4& matrix);
	Matrix4& SelfDiv(const Matrix4& matrix);
	Matrix4 Mul(const Matrix4& matrix) const;
	Matrix4 Div(const Matrix4& matrix) const;
	Matrix4 Add(const Matrix4& matrix) const;
	Matrix4 Sub(const Matrix4& matrix) const;

	Matrix4& SelfMul(const float& scalar);
	Matrix4& SelfDiv(const float& scalar);
	Matrix4 Mul(const float& scalar) const;
	Matrix4 Div(const float& scalar) const;

	Matrix4	Inverse() const;
	Matrix4	GetCFactor(int p, int q, int n) const;
	float	GetDet(int n) const;
	Matrix4 GetAdj() const;

	D3DXMATRIX ConvertToDxMatrix() const;
public:

	PROPERTY(float, _11);
	GET(_11) const { return _numbers[0][0]; }
	SET(_11) { _numbers[0][0] = value; }
	PROPERTY(float, _12);
	GET(_12) const { return _numbers[0][1]; }
	SET(_12) { _numbers[0][1] = value; }
	PROPERTY(float, _13);
	GET(_13) const { return _numbers[0][2]; }
	SET(_13) { _numbers[0][2] = value; }
	PROPERTY(float, _14);
	GET(_14) const { return _numbers[0][3]; }
	SET(_14) { _numbers[0][3] = value; }

	PROPERTY(float, _21);
	GET(_21) const { return _numbers[1][0]; }
	SET(_21) { _numbers[1][0] = value; }
	PROPERTY(float, _22);
	GET(_22) const { return _numbers[1][1]; }
	SET(_22) { _numbers[1][1] = value; }
	PROPERTY(float, _23);
	GET(_23) const { return _numbers[1][2]; }
	SET(_23) { _numbers[1][2] = value; }
	PROPERTY(float, _24);
	GET(_24) const { return _numbers[1][3]; }
	SET(_24) { _numbers[1][3] = value; }

	PROPERTY(float, _31);
	GET(_31) const { return _numbers[2][0]; }
	SET(_31) { _numbers[2][0] = value; }
	PROPERTY(float, _32);
	GET(_32) const { return _numbers[2][1]; }
	SET(_32) { _numbers[2][1] = value; }
	PROPERTY(float, _33);
	GET(_33) const { return _numbers[2][2]; }
	SET(_33) { _numbers[2][2] = value; }
	PROPERTY(float, _34);
	GET(_34) const { return _numbers[2][3]; }
	SET(_34) { _numbers[2][3] = value; }

	PROPERTY(float, _41);
	GET(_41) const { return _numbers[3][0]; }
	SET(_41) { _numbers[3][0] = value; }
	PROPERTY(float, _42);
	GET(_42) const { return _numbers[3][1]; }
	SET(_42) { _numbers[3][1] = value; }
	PROPERTY(float, _43);
	GET(_43) const { return _numbers[3][2]; }
	SET(_43) { _numbers[3][2] = value; }
	PROPERTY(float, _44);
	GET(_44) const { return _numbers[3][3]; }
	SET(_44) { _numbers[3][3] = value; }
protected:
	float _numbers[4][4] = { 0 };
};