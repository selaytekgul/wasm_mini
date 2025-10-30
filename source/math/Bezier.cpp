//
// Created by Selay Tekgül on 29.10.2025.
//

#include "Bezier.h"
#include "raymath.h"

using namespace raylib;
Vector2 Bezier::getQuadraticPoint(Vector2 p0, Vector2 p1, Vector2 p2, float t)
{
    Vector2 v1 = Vector2Lerp(p0, p1, t);
    Vector2 v2 = Vector2Lerp(p1, p2, t);
    return Vector2Lerp(v1, v2, t);
}

Vector2 Bezier::getCubicPoint(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, float t)
{
    Vector2 v_ab = Vector2Lerp(p0, p1, t);
    Vector2 v_bc = Vector2Lerp(p1, p2, t);
    Vector2 v_cd = Vector2Lerp(p2, p3, t);
    Vector2 v_abc = Vector2Lerp(v_ab, v_bc, t);
    Vector2 v_bcd = Vector2Lerp(v_bc, v_cd, t);
    return Vector2Lerp(v_abc, v_bcd, t);
}

Vector2 Bezier::getDegree4Point(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, float t)
{
    Vector2 v_ab = Vector2Lerp(p0, p1, t);
    Vector2 v_bc = Vector2Lerp(p1, p2, t);
    Vector2 v_cd = Vector2Lerp(p2, p3, t);
    Vector2 v_de = Vector2Lerp(p3, p4, t);
    Vector2 v_abc = Vector2Lerp(v_ab, v_bc, t);
    Vector2 v_bcd = Vector2Lerp(v_bc, v_cd, t);
    Vector2 v_cde = Vector2Lerp(v_cd, v_de, t);
    Vector2 v_abcd = Vector2Lerp(v_abc, v_bcd, t);
    Vector2 v_bcde = Vector2Lerp(v_bcd, v_cde, t);
    return Vector2Lerp(v_abcd, v_bcde, t);
}

Vector2 Bezier::getDegree5Point(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, Vector2 p5, float t)
{
    Vector2 v_ab = Vector2Lerp(p0, p1, t);
    Vector2 v_bc = Vector2Lerp(p1, p2, t);
    Vector2 v_cd = Vector2Lerp(p2, p3, t);
    Vector2 v_de = Vector2Lerp(p3, p4, t);
    Vector2 v_ef = Vector2Lerp(p4, p5, t);
    Vector2 v_abc = Vector2Lerp(v_ab, v_bc, t);
    Vector2 v_bcd = Vector2Lerp(v_bc, v_cd, t);
    Vector2 v_cde = Vector2Lerp(v_cd, v_de, t);
    Vector2 v_def = Vector2Lerp(v_de, v_ef, t);
    Vector2 v_abcd = Vector2Lerp(v_abc, v_bcd, t);
    Vector2 v_bcde = Vector2Lerp(v_bcd, v_cde, t);
    Vector2 v_cdef = Vector2Lerp(v_cde, v_def, t);
    Vector2 v_abcde = Vector2Lerp(v_abcd, v_bcde, t);
    Vector2 v_bcdef = Vector2Lerp(v_bcde, v_cdef, t);
    return Vector2Lerp(v_abcde, v_bcdef, t);
}

void Bezier::drawQuadraticCurve(Vector2 p0, Vector2 p1, Vector2 p2, float thickness, Color color, int segments)
{
    if (segments < 1) segments = 1;
    Vector2 oldPoint = p0;
    for (int i = 1; i <= segments; i++)
    {
        float t = (float)i / (float)segments;
        Vector2 newPoint = getQuadraticPoint(p0, p1, p2, t);
        DrawLineEx(oldPoint, newPoint, thickness, color);
        oldPoint = newPoint;
    }
}


void Bezier::drawQuadraticCurveUsingSlider(Vector2 p0, Vector2 p1, Vector2 p2, float thickness, Color color, int segments, int slider)
{
    if (segments < 1) segments = 1;
    Vector2 oldPoint = p0;
    for (int i = 1; i <= slider; i++)
    {
        float t = static_cast<float>(i) / static_cast<float>(segments);
        Vector2 newPoint = getQuadraticPoint(p0, p1, p2, t);
        DrawLineEx(oldPoint, newPoint, thickness, color);
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
        Vector2 newPoint = getCubicPoint(p0, p1, p2, p3, t);
        DrawLineEx(oldPoint, newPoint, thickness, color);
        oldPoint = newPoint;
    }
}

void Bezier::drawDegree4Curve(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, float thickness, Color color, int segments)
{
    if (segments < 1) segments = 1;

    Vector2 oldPoint = p0;

    for (int i = 1; i <= segments; i++)
    {
        float t = (float)i / (float)segments;
        Vector2 newPoint = getDegree4Point(p0, p1, p2, p3, p4, t);
        DrawLineEx(oldPoint, newPoint, thickness, color);
        oldPoint = newPoint;
    }
}

void Bezier::drawDegree5Curve(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, Vector2 p5, float thickness, Color color, int segments)
{
    if (segments < 1) segments = 1;

    Vector2 oldPoint = p0;

    for (int i = 1; i <= segments; i++)
    {
        float t = (float)i / (float)segments;
        Vector2 newPoint = getDegree5Point(p0, p1, p2, p3, p4, p5, t);
        DrawLineEx(oldPoint, newPoint, thickness, color);
        oldPoint = newPoint;
    }
}