//
// Created by Selay Tekgül on 29.10.2025.
//

#pragma once

#include "raylib.h"

using namespace raylib;
class Bezier
{
public:
    Bezier() = default;
    void drawQuadraticCurve(Vector2 p0, Vector2 p1, Vector2 p2, float thickness, Color color, int segments = 30);
    void drawCubicCurve(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, float thickness, Color color, int segments = 40);
    void drawDegree4Curve(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, float thickness, Color color, int segments);
private:
    Vector2 getQuadraticPoint(Vector2 p0, Vector2 p1, Vector2 p2, float t);
    Vector2 getCubicPoint(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, float t);
    Vector2 getDegree4Point(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, float t);

};