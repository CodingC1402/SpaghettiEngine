#include "Plane2D.h"

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

Plane2D::Point::Point( const POINTS& swpt )
{
	x = swpt.x;
	y = swpt.y;
}

Plane2D::Point::Point( const POINT& wpt )
{
	x = wpt.x;
	y = wpt.y;
}

inline Point Point::operator+( const Point& pt )
{
	return Point( x + pt.x, y + pt.y );
}

inline Point Point::operator-( const Point& pt )
{
	return Point( x - pt.x, y - pt.y );
}

inline void  Point::operator= ( const Point& pt )
{
	x = pt.x;
	y = pt.y;
}

inline void Plane2D::Point::operator=( const POINT& wpt )
{
	x = wpt.x;
	y = wpt.y;
}

void Plane2D::Point::operator=( const POINTS& swpt )
{
	x = swpt.x;
	y = swpt.y;
}

Plane2D::Point::operator PointF()
{
	return PointF( x, y );
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

Plane2D::PointF::PointF( const POINTS& swpt )
{
	x = swpt.x;
	y = swpt.y;
}

Plane2D::PointF::PointF( const POINT& wpt )
{
	x = wpt.x;
	y = wpt.y;
}

inline PointF PointF::operator+( const PointF& pt )
{
	return PointF( x + pt.x, y + pt.y );
}

inline PointF PointF::operator-( const PointF& pt )
{
	return PointF( x - pt.x, y - pt.y );
}

inline void  PointF::operator= ( const PointF& pt )
{
	x = pt.x;
	y = pt.y;
}

inline void Plane2D::PointF::operator=( const POINT& wpt )
{
	x = wpt.x;
	y = wpt.y;
}

void Plane2D::PointF::operator=( const POINTS& swpt )
{
	x = swpt.x;
	y = swpt.y;
}

Plane2D::PointF::operator Point()
{
	return Point( static_cast<int>(x), static_cast<int>(y) );
}
#pragma endregion