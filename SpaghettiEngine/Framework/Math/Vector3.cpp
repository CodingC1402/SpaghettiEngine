#include "Vector3.h"
#include <cmath>
#include <algorithm>
#include <iterator>

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

const float& Vector3::operator[](unsigned index) const
{
	return _numbers[index];
}

float& Vector3::operator[](unsigned index)
{
	return _numbers[index];
}

bool Vector3::operator==(const Vector3& vec) const
{
	return (x == vec.x && y == vec.y && z == vec.z);
}

bool Vector3::operator!=(const Vector3& vec) const
{
	return (x != vec.x || y != vec.y || z != vec.z);
}

Vector3& Vector3::operator=(const Vector3& vec)
{
	std::copy(std::begin(vec._numbers), std::end(vec._numbers), std::begin(_numbers));
	return *this;
}

Vector3 Vector3::operator+(const Vector3& vec) const
{
	return this->Add(vec);
}

Vector3 Vector3::operator-(const Vector3& vec) const
{
	return this->Subtract(vec);
}

Vector3 Vector3::operator*(const float& scalar) const
{
	return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3 Vector3::operator/(const float& scalar) const
{
	return Vector3(x / scalar, y / scalar, z / scalar);
}

Vector3 Vector3::operator*(const Matrix4& matrix) const
{
	return this->Mul(matrix);
}

Vector3& Vector3::operator+=(const Vector3& vec)
{
	return *this = this->Add(vec);
}

Vector3& Vector3::operator-=(const Vector3& vec)
{
	return *this = this->Subtract(vec);
}

Vector3& Vector3::operator*=(const float& scalar)
{
	return *this = this->Mul(scalar);
}

Vector3& Vector3::operator/=(const float& scalar)
{
	return *this = this->Div(scalar);
}

Vector3 Vector3::Add(const Vector3& vec) const
{
	return Vector3(x + vec.x, y + vec.y, z + vec.z);
}

Vector3 Vector3::Subtract(const Vector3& vec) const
{
	return Vector3(x - vec.x, y - vec.y, z - vec.z);
}

float Vector3::Dot(const Vector3& vec) const
{
	return (x * vec.x) + (y * vec.y) + (z * vec.z);
}

Vector3 Vector3::GetUnitVector() const
{
	float mag = GetMagnitude();
	return Div(mag);
}

Vector3 Vector3::Div(const float& scalar) const
{
	return Vector3(x / scalar, y / scalar, z / scalar);
}

Vector3 Vector3::Mul(const float& scalar) const
{
	return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3 Vector3::Mul(const Matrix4& matrix) const
{
	float vec4[4];
	vec4[0] = x;
	vec4[1] = y;
	vec4[2] = z;
	vec4[3] = 1;

	Vector3 rValue;

	for (unsigned col = 0; col < 4; col++)
		for (unsigned row = 0; row < 4; row++)
		{
			rValue[col] += vec4[row] * matrix[row][col];
		}

	return rValue;
}

Vector3 Vector3::Round()
{
	return Vector3(std::roundf(x), std::roundf(y), std::roundf(z));
}

float Vector3::GetMagnitude() const
{
	return sqrt(GetPow2Magnitude());
}

float Vector3::GetPow2Magnitude() const
{
	return (x * x) + (y * y) + (z * z);
}

D3DXVECTOR3 Vector3::ConvertToDxVector() const
{
	return D3DXVECTOR3(x, y, z);
}

Vector3 operator-(Vector3& vec)
{
	return Vector3(-vec.x, -vec.y, -vec.z);
}
