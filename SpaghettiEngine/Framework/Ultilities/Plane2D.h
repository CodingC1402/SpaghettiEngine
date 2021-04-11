#pragma once
#include "CornWnd.h"

namespace Plane2D
{
	struct PointF;
	struct Point;
	struct Size;

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

	struct Size
	{
	public:
		int width = 0;
		int height = 0;
	public:
		Size(const int& width = 0, const int& height = 0);
	};

	typedef struct Rectangle
	{
	public:
		int x = 0;
		int y = 0;
		int w = 0;
		int h = 0;
	public:
		Rectangle(const int& width = 0, const int& height = 0, const int& x = 0, const int& y = 0);
	} Rect;
}


