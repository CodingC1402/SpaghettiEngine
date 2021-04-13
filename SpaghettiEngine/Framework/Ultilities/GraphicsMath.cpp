#include "GraphicsMath.h"

void GraphicsMath::TransformVector3(Vector3* outVec, const Vector3* inVec, PMatrix transformMatrix)
{
	Vector4 tempValue;
	D3DXVec3Transform(&tempValue, inVec, transformMatrix);
	outVec->x = tempValue.x;
	outVec->y = tempValue.y;
	outVec->z = tempValue.z;
}

PMatrix GraphicsMath::NewMatrix()
{
	return new Matrix(
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	);
}

void GraphicsMath::ZeroMatrix(Matrix* outMatrix)
{
	for (int i = 0; i < 4; i++)
	{
		outMatrix[i][0] = 0;
		outMatrix[i][1] = 0;
		outMatrix[i][2] = 0;
		outMatrix[i][3] = 0;
	}
}

void GraphicsMath::RoundVector3(Vector3* vec)
{
	vec->x = static_cast<int>(vec->x + 0.5f);
	vec->y = static_cast<int>(vec->y + 0.5f);
	vec->z = static_cast<int>(vec->z + 0.5f);
}
