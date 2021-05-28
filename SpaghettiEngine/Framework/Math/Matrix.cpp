#include "Matrix.h"
#include "SMath.h"
#include <cmath>
#include <iterator>
#include <algorithm>

constexpr auto n = 4;

Matrix4::Matrix4(const float numbers[])
{
	for (int r = 0; r < n; r++)
		for (int c = 0; c < n; c++)
			_numbers[r][c] = numbers[r * n + c];
}

const float* Matrix4::operator[](unsigned row) const
{
	return _numbers[row];
}

float* Matrix4::operator[](unsigned row)
{
	return _numbers[row];
}

Matrix4 Matrix4::GetDiagonalMatrix()
{
	float numbers[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	return Matrix4(numbers);
}

Matrix4& Matrix4::operator=(const Matrix4& matrix)
{
	for (int i = 0; i < n; i++)
		std::copy(std::begin(matrix._numbers[i]), std::end(matrix._numbers[i]), std::begin(_numbers[i]));
	return *this;
}

Matrix4& Matrix4::operator*=(const Matrix4& matrix)
{
	return this->SelfMul(matrix);
}

Matrix4& Matrix4::operator/=(const Matrix4& matrix)
{
	return this->SelfDiv(matrix);
}

Matrix4 Matrix4::operator*(const Matrix4& matrix) const
{
	return this->Mul(matrix);
}

Matrix4 Matrix4::operator/(const Matrix4& matrix) const
{
	return this->Div(matrix);
}

Matrix4 Matrix4::operator+(const Matrix4& matrix) const
{
	return this->Add(matrix);
}

Matrix4 Matrix4::operator-(const Matrix4& matrix) const
{
	return this->Sub(matrix);
}

Matrix4& Matrix4::operator*=(const float& scalar)
{
	return this->SelfMul(scalar);
}

Matrix4& Matrix4::operator/=(const float& scalar)
{
	return this->SelfDiv(scalar);
}

Matrix4 Matrix4::operator*(const float& scalar) const
{
	return this->Mul(scalar);
}

Matrix4 Matrix4::operator/(const float& scalar) const
{
	return this->Div(scalar);
}

Matrix4& Matrix4::SelfMul(const Matrix4& matrix)
{
	return *this = this->Mul(matrix);
}

Matrix4& Matrix4::SelfDiv(const Matrix4& matrix)
{
	Matrix4 inverse = matrix.Inverse();
	return this->SelfMul(inverse);
}

Matrix4 Matrix4::Mul(const Matrix4& matrix) const
{
	Matrix4 product;
	for (unsigned i = 0; i < n; ++i)
		for (unsigned j = 0; j < n; ++j)
			for (unsigned k = 0; k < n; ++k)
				product[i][j] += (*this)[i][k] * matrix[k][j];
	return product;
}

Matrix4 Matrix4::Div(const Matrix4& matrix) const
{
	Matrix4 inverse = matrix.Inverse();
	return this->Mul(inverse);
}

Matrix4 Matrix4::Add(const Matrix4& matrix) const
{
	Matrix4 rValue;
	for (int r = 0; r < n; r++)
		for (int c = 0; c < n; c++)
			rValue[r][c] = (*this)[r][c] + matrix[r][c];
	return rValue;
}

Matrix4 Matrix4::Sub(const Matrix4& matrix) const
{
	Matrix4 rValue;
	for (int r = 0; r < 4; r++)
		for (int c = 0; c < 4; c++)
			rValue[r][c] = (*this)[r][c] - matrix[r][c];
	return rValue;
}

Matrix4& Matrix4::SelfMul(const float& scalar)
{
	return *this = this->Mul(scalar);
}

Matrix4& Matrix4::SelfDiv(const float& scalar)
{
	return *this = this->Div(scalar);
}

Matrix4 Matrix4::Mul(const float& scalar) const
{
	Matrix4 rValue;
	rValue = *this;
	for (int r = 0; r < n; r++)
		for (int c = 0; c < n; c++)
			rValue[r][c] *= scalar;
	return rValue;
}

Matrix4 Matrix4::Div(const float& scalar) const
{
	Matrix4 rValue;
	rValue = *this;
	for (int r = 0; r < n; r++)
		for (int c = 0; c < n; c++)
			rValue[r][c] /= scalar;
	return rValue;
}

D3DXMATRIX Matrix4::ConvertToDxMatrix() const
{
	auto dxMatrix = D3DXMATRIX();
	dxMatrix._11 = _numbers[0][0];
	dxMatrix._12 = _numbers[0][1];
	dxMatrix._13 = _numbers[0][2];
	dxMatrix._14 = _numbers[0][3];

	dxMatrix._21 = _numbers[1][0];
	dxMatrix._22 = _numbers[1][1];
	dxMatrix._23 = _numbers[1][2];
	dxMatrix._24 = _numbers[1][3];
		
	dxMatrix._31 = _numbers[2][0];
	dxMatrix._32 = _numbers[2][1];
	dxMatrix._33 = _numbers[2][2];
	dxMatrix._34 = _numbers[2][3];

	dxMatrix._41 = _numbers[3][0];
	dxMatrix._42 = _numbers[3][1];
	dxMatrix._43 = _numbers[3][2];
	dxMatrix._44 = _numbers[3][3];
	return dxMatrix;
}


Matrix4 Matrix4::Inverse() const
{
	const float det = GetDet(n);
	if (SMath::compare(det, 0.0f))
		return Matrix4();
	Matrix4 adj;
	Matrix4 rValue;
	adj = GetAdj();

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			rValue[i][j] = adj[i][j] / det;
	return rValue;
}
Matrix4 Matrix4::GetCFactor(int p, int q, int n) const
{
	Matrix4 out;
	int i = 0, j = 0;
	for (int r = 0; r < n; r++) {
		for (int c = 0; c < n; c++) //Copy only those elements which are not in given row r and column c: {
			if (r != p && c != q) {
				out[i][j] = (*this)[r][c]; //If row is filled increase r index and reset c index
				j++;
				if (j == n - 1) {
					j = 0; i++;
				}
			}
	}
	return out;
}
float Matrix4::GetDet(int n) const
{
	float D = 0;
	if (n == 1)
		return _numbers[0][0];
	float s = 1;
	Matrix4 tempMatrix;
	for (int f = 0; f < n; f++)
	{
		tempMatrix = GetCFactor(0, f, n);
		D += s * _numbers[0][f] * tempMatrix.GetDet(n - 1);
		s *= -1;
	}
	return D;
}

Matrix4 Matrix4::GetAdj() const
{
	int s = 1;
	constexpr int N = n;

	Matrix4 out;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			Matrix4 tempMatrix = GetCFactor(i, j, N);
			s = ((i + j) % 2 == 0) ? 1 : -1; //sign of adj[j][i] positive if sum of row and column indexes is even.
			out[j][i] = SMath::ToFloat(s) * (tempMatrix.GetDet(N - 1)); //Interchange rows and columns to get the transpose of the co factor matrix
		}
	}
	return out;
}