#include "Rectangle.h"

const Vector3& Rectangle::GetBottomLeft() const noexcept
{
	return _bottomLeft;
}

const Vector3& Rectangle::GetTopRight() const noexcept
{
	return _topRight;
}

const Vector3& Rectangle::GetCenter() const noexcept
{
	return _center;
}

float Rectangle::GetMaxX() const noexcept
{
	return _topRight.x;
}

float Rectangle::GetMaxY() const noexcept
{
	return _topRight.y;
}

float Rectangle::GetMinX() const noexcept
{
	return _bottomLeft.x;
}

float Rectangle::GetMinY() const noexcept
{
	return _bottomLeft.y;
}

float Rectangle::GetWidth() const noexcept
{
	return _width;
}

float Rectangle::GetHeight() const noexcept
{
	return _height;
}

void Rectangle::SetCenter(const Vector3& center)
{
	_center = center;

	_topRight = _center;
	_bottomLeft = _center;
	_topRight.x += _width / 2.0f;
	_topRight.y += _height / 2.0f;
	_bottomLeft.x -= _width / 2.0f;
	_bottomLeft.y -= _height / 2.0f;
}

void Rectangle::SetWidth(const float& width)
{
	_topRight.x = _center.x;
	_bottomLeft.x = _center.x;

	_topRight.x += _width / 2.0f;
	_bottomLeft.x -= _width / 2.0f;
}

void Rectangle::SetHeight(const float& height)
{
	_topRight.y = _center.y;
	_bottomLeft.y = _center.y;

	_topRight.y += _height / 2.0f;
	_bottomLeft.y -= _height / 2.0f;
}

Rectangle::Rectangle(const Vector3& center, const float& width, const float& height)
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
