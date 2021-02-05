#pragma once
#include "SpaghettiWin.h"

namespace Plane2D
{
	struct PointF;
	struct Point;
	struct Vector;

	struct PointF
	{
	public:
		float x = 0;
		float y = 0;
	public:
		PointF() = default;
		PointF( const float& fX, const float& fY );
		PointF( const PointF& pt );
		PointF( const POINTS& swpt );
		PointF( const POINT& wpt );

		PointF	operator+( const PointF& pt );
		PointF	operator-( const PointF& pt );
		void	operator=( const PointF& pt );
		void	operator=( const POINT& wpt );
		void	operator=( const POINTS& swpt );
		operator Point();
	};

	struct Point
	{
	public:
		int x = 0;
		int y = 0;
	public:
		Point() = default;
		Point( const int& iX, const int& iY );
		Point( const Point& pt );
		Point( const POINTS& swpt );
		Point( const POINT& wpt );

		Point	operator+	( const Point& pt );
		Point	operator-	( const Point& pt );
		void	operator=	( const Point& pt );
		void	operator=	( const POINT& wpt );
		void	operator=	( const POINTS& swpt );
		operator PointF();
	};

	struct Vector : PointF
	{
	public:

	};
}


