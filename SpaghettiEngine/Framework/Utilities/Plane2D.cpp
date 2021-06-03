#include "Plane2D.h"
#include "CornWnd.h"

using namespace Plane2D;

#pragma region Point
Point::Point( const int& iX, const int& iY )
{
	x = iX;
	y = iY;
}

Plane2D::Point::Point( const Point& pt )
{
	x = pt.x;
	y = pt.y;
}

Plane2D::Point::Point( const tagPOINTS& swpt )
{
	x = swpt.x;
	y = swpt.y;
}

Plane2D::Point::Point( const tagPOINT& wpt )
{
	x = wpt.x;
	y = wpt.y;
}

Point Point::operator+( const Point& pt )
{
	return Point( x + pt.x, y + pt.y );
}

Point Point::operator-( const Point& pt )
{
	return Point( x - pt.x, y - pt.y );
}

void  Point::operator= ( const Point& pt )
{
	x = pt.x;
	y = pt.y;
}

void Plane2D::Point::operator=( const tagPOINT& wpt )
{
	x = wpt.x;
	y = wpt.y;
}

void Plane2D::Point::operator=( const tagPOINTS& swpt )
{
	x = swpt.x;
	y = swpt.y;
}

Plane2D::Point::operator PointF()
{
	return PointF( static_cast<float>(x), static_cast<float>(y));
}
#pragma endregion

#pragma region PointF
PointF::PointF( const float& fX, const float& fY )
{
	x = fX;
	y = fY;
}

Plane2D::PointF::PointF( const PointF& pt )
{
	x = pt.x;
	y = pt.y;
}

Plane2D::PointF::PointF( const tagPOINTS& swpt )
{
	x = swpt.x;
	y = swpt.y;
}

Plane2D::PointF::PointF( const tagPOINT& wpt )
{
	x = static_cast<float>(wpt.x);
	y = static_cast<float>(wpt.y);
}

PointF PointF::operator+( const PointF& pt )
{
	return PointF( x + pt.x, y + pt.y );
}

PointF PointF::operator-( const PointF& pt )
{
	return PointF( x - pt.x, y - pt.y );
}

void  PointF::operator= ( const PointF& pt )
{
	x = pt.x;
	y = pt.y;
}

void Plane2D::PointF::operator=( const tagPOINT& wpt )
{
	x = static_cast<float>(wpt.x);
	y = static_cast<float>(wpt.y);
}

void Plane2D::PointF::operator=( const tagPOINTS& swpt )
{
	x = swpt.x;
	y = swpt.y;
}

Plane2D::PointF::operator Point()
{
	return Point( static_cast<int>(x), static_cast<int>(y) );
}
#pragma endregion

Plane2D::Size::Size(const int& width, const int& height)
{
	this->width = width;
	this->height = height;
}

Plane2D::Rectangle::Rectangle(const int& x, const int& y, const int& width, const int& height)
	:
	x(x),
	y(y),
	w(width),
	h(height)
{}
