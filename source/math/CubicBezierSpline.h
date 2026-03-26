//
// CubicBezierSpline.h
// C1-continuous cubic Bézier spline with interactive knot/handle dragging.
//
// Each knot stores: position, handleIn (absolute), handleOut (absolute).
// Segment i uses: knots[i].pos → knots[i].handleOut → knots[i+1].handleIn → knots[i+1].pos
// C1 enforcement: moving one handle mirrors it through the knot to the other handle
// (same direction AND length).
//

#pragma once

#include "raylib.h"
#include <vector>

using namespace raylib;

// ─── Data ────────────────────────────────────────────────────────────────────

struct SplineKnot {
    Vector2 position;
    Vector2 handleIn;   // absolute world-space position
    Vector2 handleOut;  // absolute world-space position
};

enum class SelectionType { None, Knot, HandleIn, HandleOut };

struct SplineSelection {
    SelectionType type = SelectionType::None;
    int knotIndex = -1;
};

// ─── Class ───────────────────────────────────────────────────────────────────

class CubicBezierSpline {
public:
    CubicBezierSpline();

    // Call every frame in order: update → BeginDrawing → draw → drawImGui → EndDrawing
    void update();       // input handling (pick, drag)
    void draw();         // render curve + control points + handles
    void drawImGui();    // ImGui control panel

    // Programmatic knot manipulation
    void addKnot(Vector2 pos, Vector2 handleIn, Vector2 handleOut);
    void addKnotAtEnd();
    void insertKnotAfter(int index);  // inserts at midpoint of segment [index, index+1]
    void removeKnot(int index);

private:
    std::vector<SplineKnot> knots;
    SplineSelection selection;
    bool isDragging = false;
    int segmentsPerCurve = 40;
    float pickRadius = 15.0f;
    Vector2 dragOffset = {0, 0};

    static Vector2 evalCubic(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, float t);
    void enforceC1(int knotIndex, bool movedOut);
    void tryPick(Vector2 mousePos);
    void initDefaultKnots();
};