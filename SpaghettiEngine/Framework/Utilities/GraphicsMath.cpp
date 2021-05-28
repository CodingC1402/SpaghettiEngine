#include "GraphicsMath.h"
#include "ExMath.h"

float GraphicsMath::ToRad(const float& degree)
{
	return degree * (pi / 180.0f);
}

bool GraphicsMath::CompareFloat(float x, float y, float epsilon)
{
	if (fabs(x - y) < epsilon)
		return true;
	return false;
}

void GraphicsMath::TransformVector3(Vector3& outVec, Vector3& inVec, Matrix4& transformMatrix)
{
	outVec = inVec * transformMatrix;
}


void GraphicsMath::RoundVector3(Vector3* vec)
{
	vec->x = std::round(vec->x);
	vec->y = std::round(vec->y);
	vec->z = std::round(vec->z);
}

void GraphicsMath::TranslateMatrix(Matrix4& matrix, const Vector3& vec)
{
	matrix._41 += vec.x;
	matrix._42 += vec.y;
	matrix._43 += vec.z;
}

void GraphicsMath::MoveMatrix(Matrix4& matrix, const Vector3& vec)
{
	matrix._41 = vec.x;
	matrix._42 = vec.y;
	matrix._43 = vec.z;
}

void GraphicsMath::TranslateMatrix(Matrix4& matrix, const float& x, const float& y, const float& z)
{
	matrix._41 += x;
	matrix._42 += y;
	matrix._43 += z;
}

void GraphicsMath::MoveMatrix(Matrix4& matrix, const float& x, const float& y, const float& z)
{
	matrix._41 = x;
	matrix._42 = y;
	matrix._43 = z;
}

bool GraphicsMath::Inverse(const Matrix4& matrix, Matrix4& inverseMatrix)
{
	constexpr int N = 4;
	const float det = GetDet(matrix, N);
	if (CLib::compare(det, 0.0f))
		return false;
	Matrix4 adj;
	GetAdj(matrix, adj);

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			inverseMatrix[i][j] = adj[i][j] / det;
	return true;
}
void GraphicsMath::GetCFactor(const Matrix4& in, Matrix4& out, int p, int q, int n)
{
	int i = 0, j = 0;
	for (int r = 0; r < n; r++) {
		for (int c = 0; c < n; c++) //Copy only those elements which are not in given row r and column c: {
			if (r != p && c != q) {
				out[i][j] = in[r][c]; //If row is filled increase r index and reset c index
				if (j == n - 1) {
					j = 0; i++;
				}
			}
	}
}
float GraphicsMath::GetDet(const Matrix4& matrix, int n)
{
	float D = 0;
	if (n == 1)
		return matrix[0][0];
	float s = 1;
	for (int f = 0; f < n; f++) 
	{
		Matrix4 tempMatrix;
		GetCFactor(matrix, tempMatrix, 0, f, n);
		D += s * matrix[0][f] * GetDet(tempMatrix, n - 1);
		s = -s;
	}
	return D;
}

void GraphicsMath::GetAdj(const Matrix4& in, Matrix4& out)
{
	int s = 1;
	constexpr int N = 4;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			Matrix4 tempMatrix;
			GetCFactor(in, tempMatrix, i, j, N);
			s = ((i + j) % 2 == 0) ? 1 : -1; //sign of adj[j][i] positive if sum of row and column indexes is even.
			out[j][i] = CLib::ToFloat(s) * (GetDet(tempMatrix, N - 1)); //Interchange rows and columns to get the transpose of the co factor matrix
		}
	}
}

void GraphicsMath::Modulo(Vector3& out, const float& f)
{
	out.x = CLib::modulo(out.x, f);
	out.y = CLib::modulo(out.y, f);
	out.z = CLib::modulo(out.z, f);
}

Matrix4 GraphicsMath::GetXAxisRotateMatrix(const float& degree)
{
	Matrix4 XAxis;
	XAxis._11 = 1;
	XAxis._44 = 1;
	const float rad = GraphicsMath::ToRad(degree);
	XAxis._22 = std::cosf(rad);
	XAxis._23 = std::sinf(rad);
	XAxis._32 = -std::sinf(rad);
	XAxis._33 = std::cosf(rad);
	return XAxis;
}

Matrix4 GraphicsMath::GetYAxisRotateMatrix(const float& degree)
{
	Matrix4 YAxis;
	YAxis._22 = 1;
	YAxis._44 = 1;
	const float rad = GraphicsMath::ToRad(degree);
	YAxis._11 = std::cosf(rad);
	YAxis._13 = -std::sinf(rad);
	YAxis._31 = std::sinf(rad);
	YAxis._33 = std::cosf(rad);
	return YAxis;
}

Matrix4 GraphicsMath::GetZAxisRotateMatrix(const float& degree)
{
	Matrix4 ZAxis;
	ZAxis._33 = 1;
	ZAxis._44 = 1;
	const float rad = GraphicsMath::ToRad(degree);
	ZAxis._11 = std::cosf(rad);
	ZAxis._12 = std::sinf(rad);
	ZAxis._21 = -std::sinf(rad);
	ZAxis._22 = std::cosf(rad);
	return ZAxis;
}
