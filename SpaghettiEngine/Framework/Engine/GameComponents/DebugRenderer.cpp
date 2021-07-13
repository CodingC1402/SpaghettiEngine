#include "DebugRenderer.h"
#include "DirectX9Graphic.h"
#include "Setting.h"
#include "Matrix.h"
#include "SMath.h"

void DebugRenderer::DrawPolygon(const std::vector<Vector3>& vertexes, const Matrix4& matrix)
{
	if constexpr (Setting::IsDebugMode())
	{
		auto copy = vertexes;
		copy.reserve(static_cast<unsigned long long>(vertexes.size()) + 1_ut);
		copy.push_back(vertexes.front());
		_shapes.emplace_back(std::pair(std::move(copy), matrix));
	}
}

void DebugRenderer::DrawCircle(const float& radius, const Matrix4& matrix)
{
	unsigned vertexesSize = SMath::Lerp(_minVertexNumberForCircle, _maxVertexNumberForCircle, radius / _radiusToReachMaxVertexNum);
	Matrix4 rotationMatrix = SMath::GetZAxisRotateMatrix(360.0f / vertexesSize);
	std::vector<Vector3> vertexes(static_cast<unsigned long long>(vertexesSize) + 1_ut);

	vertexes.front().y = radius;

	for (unsigned i = 1; i < vertexes.size() - 1_ut; i++)
	{
		vertexes[i] = vertexes[i - 1_ut] * rotationMatrix;
	}
	vertexes.back() = vertexes.front();

	_shapes.emplace_back(std::pair(std::move(vertexes), matrix));
}

void DebugRenderer::DrawRectangle(Vector3 topLeft, float width, float height, const Matrix4& matrix)
{
	Vector3 topRight, bottomLeft, bottomRight;
	topRight.x = topLeft.x + width;
	topRight.y = topLeft.y;

	bottomLeft.y = topLeft.y - height;
	bottomRight.y = bottomLeft.y;

	bottomRight.x = topRight.x;
	bottomLeft.x = topLeft.x;

	std::vector<Vector3> vertexes = { topLeft, topRight, bottomRight, bottomLeft, topLeft };
	_shapes.emplace_back(std::move(vertexes), matrix);
}

void DebugRenderer::SetColor(const Color& color) noexcept
{
	if constexpr (Setting::IsDebugMode())
		_renderColor = color;
}

void DebugRenderer::SetWidth(const float& width) noexcept
{
	if constexpr (Setting::IsDebugMode())
		_width = width;
}

Color DebugRenderer::GetColor() noexcept
{
	return _renderColor;
}

float DebugRenderer::GetWidth() noexcept
{
	return _width;
}

void DebugRenderer::Render(SDirectX9Graphic dxTurd, PCamera cameraScript)
{
	if constexpr (Setting::IsDebugMode())
	{
		Vector3 currentVec3;
		Vector2* dxVertexes = nullptr;
		size_t size = 0;

		dxTurd->StartRenderLine();
		for (auto& shapeMatrix : _shapes)
		{
			// Kinda how vector work, if the required size is smaller than what requested then keep the size.
			if (size < shapeMatrix.first.size())
			{
				delete[] dxVertexes;
				dxVertexes = new Vector2[shapeMatrix.first.size()];
			}

			size = shapeMatrix.first.size();
			for (int i = 0; i < size; i++)
			{
				currentVec3 = shapeMatrix.first[i] * cameraScript->GetMatrixWithoutScaleY(shapeMatrix.second);
				dxVertexes[i].x = static_cast<FLOAT>(currentVec3.x);
				dxVertexes[i].y = static_cast<FLOAT>(currentVec3.y);
			}

			dxTurd->RenderPolygon(dxVertexes, size, _width, _renderColor);
		}
		dxTurd->EndRenderLine();

		delete[] dxVertexes;
		dxVertexes = nullptr;
	}
}

void DebugRenderer::Clear() noexcept
{
	if constexpr (Setting::IsDebugMode())
		_shapes.clear();
}
