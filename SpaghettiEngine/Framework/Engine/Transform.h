#pragma once
#include "CornDirectX.h"

class Transform
{
public:
	Transform();
public:
	float x;
	float y;
	float z;

protected:
	Matrix transformMatrix;
};