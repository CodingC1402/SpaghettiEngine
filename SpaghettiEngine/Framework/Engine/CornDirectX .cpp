#include "CornDirectX.h"

Vector3::Vector3(const Matrix& matrix)
{
	this->x = matrix._41;
	this->y = matrix._42;
	this->z = matrix._43;
}
