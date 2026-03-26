#pragma once

#include "raylib.h"
#include <vector>

using namespace raylib;

struct SplineKnot {
    Vector2 pos, hIn, hOut;
};

class CubicBezierSpline {
public:
    CubicBezierSpline() = default;

    std::vector<SplineKnot> knots;

    void drawSpline(int segments = 40);
    void enforceC1(int index, bool movedOut);

private:
    Vector2 getCubicPoint(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, float t);
};