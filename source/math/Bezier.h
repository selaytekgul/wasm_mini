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
private:
    Vector2 getQuadraticPoint(Vector2 p0, Vector2 p1, Vector2 p2, float t);
    Vector2 getCubicPoint(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, float t);

};