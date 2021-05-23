#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "PolygonMath.h"

static const float EPSILON = 0.0000000001f;

float Triangulate::Area(const VectorPointF& contour)
{
    int n = contour.size();

    float A = 0.0f;

    for (int p = n - 1, q = 0; q < n; p = q++)
    {
        A += contour[p].x * contour[q].y - contour[q].x * contour[p].y;
    }
    return A * 0.5f;
}

/*
  InsideTriangle decides if a point P is Inside of the triangle
  defined by A, B, C.
*/
bool Triangulate::InsideTriangle(float Ax, float Ay,
    float Bx, float By,
    float Cx, float Cy,
    float Px, float Py)
{
    float ax, ay, bx, by, cx, cy, apx, apy, bpx, bpy, cpx, cpy;
    float cCROSSap, bCROSScp, aCROSSbp;

    ax = Cx - Bx;  ay = Cy - By;
    bx = Ax - Cx;  by = Ay - Cy;
    cx = Bx - Ax;  cy = By - Ay;
    apx = Px - Ax;  apy = Py - Ay;
    bpx = Px - Bx;  bpy = Py - By;
    cpx = Px - Cx;  cpy = Py - Cy;

    aCROSSbp = ax * bpy - ay * bpx;
    cCROSSap = cx * apy - cy * apx;
    bCROSScp = bx * cpy - by * cpx;

    return ((aCROSSbp >= 0.0f) && (bCROSScp >= 0.0f) && (cCROSSap >= 0.0f));
}

float Triangulate::MaxY(Vector3 A, Vector3 B, Vector3 C)
{
    return std::max(A.y, std::max(B.y, C.y));
}

float Triangulate::MaxX(Vector3 A, Vector3 B, Vector3 C)
{
    return std::max(A.x, std::max(B.x, C.x));
}

float Triangulate::MinY(Vector3 A, Vector3 B, Vector3 C)
{
    return std::min(A.y, std::min(B.y, C.y));
}

float Triangulate::MinX(Vector3 A, Vector3 B, Vector3 C)
{
    return std::min(A.x, std::min(B.x, C.x));
}

bool Triangulate::InsideTriangle(Vector3 A, Vector3 B, Vector3 C, Vector3 P)
{
    return InsideTriangle(A.x, A.y, B.x, B.y, C.x, C.y, P.x, P.y);
}
bool Triangulate::CheckBoxInsideTriangle(Vector3 A, Vector3 B, Vector3 C, Vector3 pos, Vector3 size)
{
    bool result = false;

    result = result || Triangulate::InsideTriangle(A, B, C,
        Vector3(pos.x, pos.y, pos.z));

    result = result || Triangulate::InsideTriangle(A, B, C,
        Vector3(pos.x + size.x, pos.y, pos.z));

    result = result || Triangulate::InsideTriangle(A, B, C,
        Vector3(pos.x, pos.y + size.y, pos.z));

    result = result || Triangulate::InsideTriangle(A, B, C,
        Vector3(pos.x + size.x, pos.y + size.y, pos.z));

    return result;
}

bool Triangulate::CheckLineIntersect(Vector3 p1, Vector3 q1, Vector3 p2, Vector3 q2)
{
    // Find the four orientations needed for general and
    // special cases
    int o1 = Orientation(p1, q1, p2);
    int o2 = Orientation(p1, q1, q2);
    int o3 = Orientation(p2, q2, p1);
    int o4 = Orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && Segment(p1, p2, q1)) return true;

    // p1, q1 and q2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && Segment(p1, q2, q1)) return true;

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && Segment(p2, p1, q2)) return true;

    // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && Segment(p2, q1, q2)) return true;

    return false; // Doesn't fall in any of the above cases}
}

bool Triangulate::Snip(const VectorPointF& contour, int u, int v, int w, int n, int* V)
{
    int p;
    float Ax, Ay, Bx, By, Cx, Cy, Px, Py;

    Ax = contour[V[u]].x;
    Ay = contour[V[u]].y;

    Bx = contour[V[v]].x;
    By = contour[V[v]].y;

    Cx = contour[V[w]].x;
    Cy = contour[V[w]].y;

    if (EPSILON > (((Bx - Ax) * (Cy - Ay)) - ((By - Ay) * (Cx - Ax)))) return false;

    for (p = 0; p < n; p++)
    {
        if ((p == u) || (p == v) || (p == w)) continue;
        Px = contour[V[p]].x;
        Py = contour[V[p]].y;
        if (InsideTriangle(Ax, Ay, Bx, By, Cx, Cy, Px, Py)) return false;
    }

    return true;
}

bool Triangulate::Segment(Vector3 p, Vector3 q, Vector3 r)
{
    if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
        return true;

    return false;
}

int Triangulate::Orientation(Vector3 p, Vector3 q, Vector3 r)
{
    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/
    // for details of below formula.
    int val = (q.y - p.y) * (r.x - q.x) -
        (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;  // colinear

    return (val > 0) ? 1 : 2; // clock or counterclock wise
}

bool Triangulate::Process(const VectorPointF& contour, VectorPointF& result)
{
    /* allocate and initialize list of Vertices in polygon */

    int n = contour.size();
    if (n < 3) return false;

    int* V = new int[n];

    /* we want a counter-clockwise polygon in V */

    if (0.0f < Area(contour))
        for (int v = 0; v < n; v++) V[v] = v;
    else
        for (int v = 0; v < n; v++) V[v] = (n - 1) - v;

    int nv = n;

    /*  remove nv-2 Vertices, creating 1 triangle every time */
    int count = 2 * nv;   /* error detection */

    for (int m = 0, v = nv - 1; nv > 2; )
    {
        /* if we loop, it is probably a non-simple polygon */
        if (0 >= (count--))
        {
            //** Triangulate: ERROR - probable bad polygon!
            return false;
        }

        /* three consecutive vertices in current polygon, <u,v,w> */
        int u = v; if (nv <= u) u = 0;     /* previous */
        v = u + 1; if (nv <= v) v = 0;     /* new v    */
        int w = v + 1; if (nv <= w) w = 0;     /* next     */

        if (Snip(contour, u, v, w, nv, V))
        {
            int a, b, c, s, t;

            /* true names of the vertices */
            a = V[u]; b = V[v]; c = V[w];

            /* output Triangle */
            result.push_back(contour[a]);
            result.push_back(contour[b]);
            result.push_back(contour[c]);

            m++;

            /* remove v from remaining polygon */
            for (s = v, t = v + 1; t < nv; s++, t++) V[s] = V[t]; nv--;

            /* resest error detection counter */
            count = 2 * nv;
        }
    }

    delete V;

    return true;
}

