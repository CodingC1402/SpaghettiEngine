#include "CRectangle.h"

const Vector3& CRectangle::GetBottomLeft() const noexcept
{
	return _bottomLeft;
}

const Vector3& CRectangle::GetTopRight() const noexcept
{
	return _topRight;
}

const Vector3& CRectangle::GetCenter() const noexcept
{
	return _center;
}

float CRectangle::GetMaxX() const noexcept
{
	return _topRight.x;
}

float CRectangle::GetMaxY() const noexcept
{
	return _topRight.y;
}

float CRectangle::GetMinX() const noexcept
{
	return _bottomLeft.x;
}

float CRectangle::GetMinY() const noexcept
{
	return _bottomLeft.y;
}

float CRectangle::GetWidth() const noexcept
{
	return _width;
}

float CRectangle::GetHeight() const noexcept
{
	return _height;
}

void CRectangle::SetCenter(const Vector3& center)
{
	_center = center;

	_topRight = _center;
	_bottomLeft = _center;
	_topRight.x += _width / 2.0f;
	_topRight.y += _height / 2.0f;
	_bottomLeft.x -= _width / 2.0f;
	_bottomLeft.y -= _height / 2.0f;
}

void CRectangle::SetWidth(const float& width)
{
	_topRight.x = _center.x;
	_bottomLeft.x = _center.x;

	_topRight.x += _width / 2.0f;
	_bottomLeft.x -= _width / 2.0f;
}

void CRectangle::SetHeight(const float& height)
{
	_topRight.y = _center.y;
	_bottomLeft.y = _center.y;

	_topRight.y += _height / 2.0f;
	_bottomLeft.y -= _height / 2.0f;
}

CRectangle::CRectangle(const Vector3& center, const float& width, const float& height)
{
	_center = center;
	_width = width;
	_height = height;

	_topRight = _center;
	_bottomLeft = _center;

	_topRight.x += _width / 2.0f;
	_topRight.y += _height / 2.0f;
	_bottomLeft.x -= _width / 2.0f;
	_bottomLeft.y -= _height / 2.0f;
}
