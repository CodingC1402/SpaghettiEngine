#pragma once

#include <vector>  // Include STL vector class.
#include "Plane2D.h"
#include "CornDirectX.h"

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

    static float MaxY(Vector3 A, Vector3 B, Vector3 C);
    static float MaxX(Vector3 A, Vector3 B, Vector3 C);
    static float MinY(Vector3 A, Vector3 B, Vector3 C);
    static float MinX(Vector3 A, Vector3 B, Vector3 C);


    static bool InsideTriangle(Vector3 A, Vector3 B, Vector3 C, Vector3 P);
    static bool CheckBoxInsideTriangle(Vector3 A, Vector3 B, Vector3 C, Vector3 pos, Vector3 size);
    static bool CheckLineIntersect(Vector3 p1, Vector3 q1, Vector3 p2, Vector3 q2);

private:
    static bool Snip(const VectorPointF& contour, int u, int v, int w, int n, int* V);
    static bool Segment(Vector3 p, Vector3 q, Vector3 r);
    static int Orientation(Vector3 p, Vector3 q, Vector3 r);

};