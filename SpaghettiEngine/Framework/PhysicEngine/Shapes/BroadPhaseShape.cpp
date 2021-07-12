#include "BroadPhaseShape.h"

void BroadPhaseShape::SetRadius(float radius) noexcept
{
	_topLeft.x = -radius;
	_topLeft.y = radius;

	_bottomRight.x = radius;
	_bottomRight.y = -radius;

	SetCenter();
}

void BroadPhaseShape::SetVertexes(const std::vector<Vector3>& vertexes) noexcept
{
	if (vertexes.empty())
		return;

	_topLeft = vertexes[0];
	_bottomRight = vertexes[0];

	for (int i = 1; i < vertexes.size(); i++)
	{
		if (_topLeft.x > vertexes[i].x)
			_topLeft.x = vertexes[i].x;
		if (_topLeft.y < vertexes[i].y)
			_topLeft.y = vertexes[i].y;

		if (_bottomRight.x < vertexes[i].x)
			_bottomRight.x = vertexes[i].x;
		if (_bottomRight.y > vertexes[i].y)
			_bottomRight.y = vertexes[i].y;
	}

	SetCenter();
}

void BroadPhaseShape::UpdateMatrix(const Matrix4& worldMatrix, const Matrix4& offSet) noexcept
{
	_worldBottomRight = _bottomRight * offSet * worldMatrix;
	_worldTopLeft = _topLeft * offSet * worldMatrix;
	_worldCenter = _center * offSet * worldMatrix;
}

const Vector3& BroadPhaseShape::GetCenter() const noexcept
{
	return _center;
}

const Vector3& BroadPhaseShape::GetTopLeft() const noexcept
{
	return _topLeft;
}

const Vector3& BroadPhaseShape::GetBottomRight() const noexcept
{
	return _bottomRight;
}

const Vector3& BroadPhaseShape::GetWorldCenter() const noexcept
{
	return _worldCenter;
}

const Vector3& BroadPhaseShape::GetWorldTopLeft() const noexcept
{
	return _worldTopLeft;
}

const Vector3& BroadPhaseShape::GetWorldBottomRight() const noexcept
{
	return _worldBottomRight;
}

float BroadPhaseShape::GetWidth() const noexcept
{
	return _width;
}

float BroadPhaseShape::GetHeight() const noexcept
{
	return _height;
}

void BroadPhaseShape::SetCenter() noexcept
{
	_center.x = (_topLeft.x + _bottomRight.x) / 2.0f;
	_center.y = (_topLeft.y + _bottomRight.y) / 2.0f;

	_width = _bottomRight.x - _topLeft.x;
	_height = _topLeft.y - _bottomRight.y;
}
