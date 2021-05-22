#pragma once

#include <vector>  // Include STL vector class.
#include "Plane2D.h"

// Typedef an STL vector of vertices which are used to represent
// a polygon/contour and a series of triangles.
typedef std::vector<Plane2D::PointF> VectorPointF;


class Triangulate
{
public:

    // triangulate a contour/polygon, places results in STL vector
    // as series of triangles.
    static bool Process(const VectorPointF& contour,
        VectorPointF& result);

    // compute area of a contour/polygon
    static float Area(const VectorPointF& contour);

    // decide if point Px/Py is inside triangle defined by
    // (Ax,Ay) (Bx,By) (Cx,Cy)
    static bool InsideTriangle(float Ax, float Ay,
        float Bx, float By,
        float Cx, float Cy,
        float Px, float Py);


private:
    static bool Snip(const VectorPointF& contour, int u, int v, int w, int n, int* V);

};