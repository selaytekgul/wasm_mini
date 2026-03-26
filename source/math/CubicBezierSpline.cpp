#include "CubicBezierSpline.h"
#include "raymath.h"

using namespace raylib;

Vector2 CubicBezierSpline::getCubicPoint(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, float t)
{
    Vector2 ab  = Vector2Lerp(p0, p1, t);
    Vector2 bc  = Vector2Lerp(p1, p2, t);
    Vector2 cd  = Vector2Lerp(p2, p3, t);
    Vector2 abc = Vector2Lerp(ab, bc, t);
    Vector2 bcd = Vector2Lerp(bc, cd, t);
    return Vector2Lerp(abc, bcd, t);
}

void CubicBezierSpline::drawSpline(int segments)
{
    if (segments < 1) segments = 1;

    for (int seg = 0; seg < (int)knots.size() - 1; seg++)
    {
        Vector2 p0 = knots[seg].pos;
        Vector2 p1 = knots[seg].hOut;
        Vector2 p2 = knots[seg + 1].hIn;
        Vector2 p3 = knots[seg + 1].pos;

        Vector2 oldPoint = p0;
        for (int i = 1; i <= segments; i++)
        {
            float t = (float)i / (float)segments;
            Vector2 newPoint = getCubicPoint(p0, p1, p2, p3, t);
            DrawLineEx(oldPoint, newPoint, 3.0f, WHITE);
            oldPoint = newPoint;
        }
    }
}

void CubicBezierSpline::enforceC1(int index, bool movedOut)
{
    SplineKnot& k = knots[index];
    if (movedOut)
        k.hIn = { 2 * k.pos.x - k.hOut.x, 2 * k.pos.y - k.hOut.y };
    else
        k.hOut = { 2 * k.pos.x - k.hIn.x, 2 * k.pos.y - k.hIn.y };
}