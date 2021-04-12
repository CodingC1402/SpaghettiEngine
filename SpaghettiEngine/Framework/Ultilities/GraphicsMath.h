#pragma once
#include "CornDirectX.h"

/// <summary>
/// In the name of jusus the korean god, why do I have to do this,
/// why do I have to use directx9, it's so freaking janky >:((
/// </summary>

class GraphicsMath
{
public:
	static void TransformVector3(Vector3* outVec, const Vector3* inVec, PMatrix transformMatrix);
	static PMatrix NewMatrix(); // Just WHY?, for god sake >:(
	static void ZeroMatrix(Matrix* outMatrix);
	static void RoundVector3(Vector3* vec);
};