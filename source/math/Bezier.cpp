//
// Created by Selay Tekgül on 29.10.2025.
//

#include "Bezier.h"
#include "raymath.h"

using namespace raylib;
Vector2 Bezier::getQuadraticPoint(Vector2 p0, Vector2 p1, Vector2 p2, float t)
{
    // Level 1 of interpolation:
    // Find a point (v1) on the line segment from P0 to P1
    Vector2 v1 = Vector2Lerp(p0, p1, t);
    // Find a point (v2) on the line segment from P1 to P2
    Vector2 v2 = Vector2Lerp(p1, p2, t);

    // Level 2 of interpolation:
    // Find the point on the line segment from v1 to v2
    return Vector2Lerp(v1, v2, t);
}

Vector2 Bezier::getCubicPoint(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, float t)
{
    // Level 1 of interpolation (on the 3 control legs):
    Vector2 v_ab = Vector2Lerp(p0, p1, t);
    Vector2 v_bc = Vector2Lerp(p1, p2, t);
    Vector2 v_cd = Vector2Lerp(p2, p3, t);

    // Level 2 of interpolation (on the 2 new "virtual" legs):
    Vector2 v_abc = Vector2Lerp(v_ab, v_bc, t);
    Vector2 v_bcd = Vector2Lerp(v_bc, v_cd, t);

    // Level 3 of interpolation (the final point):
    return Vector2Lerp(v_abc, v_bcd, t);
}

void Bezier::drawQuadraticCurve(Vector2 p0, Vector2 p1, Vector2 p2, float thickness, Color color, int segments)
{
    if (segments < 1) segments = 1;

    // We start at the beginning of the curve
    Vector2 oldPoint = p0;

    for (int i = 1; i <= segments; i++)
    {
        // Calculate our 't' value for this segment
        float t = (float)i / (float)segments;

        // Get the new point on the curve
        Vector2 newPoint = getQuadraticPoint(p0, p1, p2, t);

        // Draw a small line from the last point to the new one
        DrawLineEx(oldPoint, newPoint, thickness, color);

        // This new point is now the "old" point for the next loop
        oldPoint = newPoint;
    }
}

void Bezier::drawCubicCurve(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, float thickness, Color color, int segments)
{
    if (segments < 1) segments = 1;

    Vector2 oldPoint = p0;

    for (int i = 1; i <= segments; i++)
    {
        float t = (float)i / (float)segments;

        // Get the new point (this is the only line different from the quadratic func)
        Vector2 newPoint = getCubicPoint(p0, p1, p2, p3, t);

        DrawLineEx(oldPoint, newPoint, thickness, color);

        oldPoint = newPoint;
    }
}