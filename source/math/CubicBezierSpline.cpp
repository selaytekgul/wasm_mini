//
// CubicBezierSpline.cpp
//

#include "CubicBezierSpline.h"
#include "raymath.h"
#include "imgui.h"
#include <cmath>
#include <cstdio>
#include <algorithm>

using namespace raylib;

// ─── Construction ────────────────────────────────────────────────────────────

CubicBezierSpline::CubicBezierSpline() {
    initDefaultKnots();
}

void CubicBezierSpline::initDefaultKnots() {
    knots.clear();
    constexpr float screenW  = 1280.0f;
    constexpr float screenH  = 720.0f;
    constexpr float margin   = 150.0f;
    constexpr int   count    = 5;
    const float spacing    = (screenW - 2.0f * margin) / (float)(count - 1);
    const float handleLen  = spacing * 0.33f;

    for (int i = 0; i < count; i++) {
        SplineKnot k;
        float x = margin + i * spacing;
        // Zigzag vertically so the default curve is interesting
        float y = screenH / 2.0f + ((i % 2 == 0) ? -100.0f : 100.0f);
        k.position  = { x, y };
        k.handleIn  = { x - handleLen, y };
        k.handleOut = { x + handleLen, y };
        knots.push_back(k);
    }
}

// ─── Evaluation ──────────────────────────────────────────────────────────────

Vector2 CubicBezierSpline::evalCubic(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, float t) {
    // Bernstein polynomial form
    float u   = 1.0f - t;
    float uu  = u * u;
    float uuu = uu * u;
    float tt  = t * t;
    float ttt = tt * t;
    return {
        uuu * p0.x + 3.0f * uu * t * p1.x + 3.0f * u * tt * p2.x + ttt * p3.x,
        uuu * p0.y + 3.0f * uu * t * p1.y + 3.0f * u * tt * p2.y + ttt * p3.y
    };
}

// ─── C1 Enforcement ─────────────────────────────────────────────────────────

void CubicBezierSpline::enforceC1(int idx, bool movedOut) {
    SplineKnot& k = knots[idx];
    if (movedOut) {
        // We moved handleOut → mirror through position to produce handleIn
        Vector2 diff = { k.handleOut.x - k.position.x,
                         k.handleOut.y - k.position.y };
        k.handleIn = { k.position.x - diff.x, k.position.y - diff.y };
    } else {
        // We moved handleIn → mirror through position to produce handleOut
        Vector2 diff = { k.handleIn.x - k.position.x,
                         k.handleIn.y - k.position.y };
        k.handleOut = { k.position.x - diff.x, k.position.y - diff.y };
    }
}

// ─── Picking ────────────────────────────────────────────────────────────────

void CubicBezierSpline::tryPick(Vector2 mousePos) {
    float bestDist = pickRadius;
    selection = { SelectionType::None, -1 };

    for (int i = 0; i < (int)knots.size(); i++) {
        // Check handles first (smaller targets, give them priority at equal dist)
        float dIn = Vector2Distance(mousePos, knots[i].handleIn);
        if (dIn < bestDist) {
            bestDist  = dIn;
            selection = { SelectionType::HandleIn, i };
        }
        float dOut = Vector2Distance(mousePos, knots[i].handleOut);
        if (dOut < bestDist) {
            bestDist  = dOut;
            selection = { SelectionType::HandleOut, i };
        }
        // Knot itself — higher priority if equally close
        float dKnot = Vector2Distance(mousePos, knots[i].position);
        if (dKnot < bestDist) {
            bestDist  = dKnot;
            selection = { SelectionType::Knot, i };
        }
    }
}

// ─── Input Handling ─────────────────────────────────────────────────────────

void CubicBezierSpline::update() {
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) return;          // ImGui has focus

    Vector2 mousePos = GetMousePosition();

    // --- Press: pick closest element ---
    if (IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT)) {
        tryPick(mousePos);
        if (selection.type != SelectionType::None) {
            isDragging = true;
            // Offset so the point doesn't snap to cursor center
            Vector2 selPos;
            switch (selection.type) {
                case SelectionType::Knot:      selPos = knots[selection.knotIndex].position;  break;
                case SelectionType::HandleIn:  selPos = knots[selection.knotIndex].handleIn;  break;
                case SelectionType::HandleOut: selPos = knots[selection.knotIndex].handleOut; break;
                default: selPos = mousePos; break;
            }
            dragOffset = { selPos.x - mousePos.x, selPos.y - mousePos.y };
        }
    }

    // --- Drag: move the selected element ---
    if (isDragging && IsMouseButtonDown(MouseButton::MOUSE_BUTTON_LEFT)) {
        Vector2 target = { mousePos.x + dragOffset.x, mousePos.y + dragOffset.y };
        int idx = selection.knotIndex;

        switch (selection.type) {
            case SelectionType::Knot: {
                // Translate knot + both handles together
                Vector2 delta = { target.x - knots[idx].position.x,
                                  target.y - knots[idx].position.y };
                knots[idx].position.x  += delta.x;
                knots[idx].position.y  += delta.y;
                knots[idx].handleIn.x  += delta.x;
                knots[idx].handleIn.y  += delta.y;
                knots[idx].handleOut.x += delta.x;
                knots[idx].handleOut.y += delta.y;
                break;
            }
            case SelectionType::HandleIn: {
                knots[idx].handleIn = target;
                enforceC1(idx, false);   // moved In → mirror to Out
                break;
            }
            case SelectionType::HandleOut: {
                knots[idx].handleOut = target;
                enforceC1(idx, true);    // moved Out → mirror to In
                break;
            }
            default: break;
        }
    }

    // --- Release ---
    if (IsMouseButtonReleased(MouseButton::MOUSE_BUTTON_LEFT)) {
        isDragging = false;
    }
}

// ─── Drawing ────────────────────────────────────────────────────────────────

void CubicBezierSpline::draw() {
    const Color curveColor       = WHITE;
    const Color handleLineColor  = { 100, 100, 100, 180 };
    const Color handleDotDefault = { 180, 180, 180, 255 };
    const Color handleDotActive  = YELLOW;
    const Color knotOuter        = { 50, 150, 255, 255 };
    const Color knotInner        = { 30, 100, 200, 255 };
    const Color knotActiveOuter  = YELLOW;
    const Color knotActiveInner  = WHITE;

    // --- Curve segments ---
    for (int seg = 0; seg < (int)knots.size() - 1; seg++) {
        Vector2 p0 = knots[seg].position;
        Vector2 p1 = knots[seg].handleOut;
        Vector2 p2 = knots[seg + 1].handleIn;
        Vector2 p3 = knots[seg + 1].position;

        Vector2 prev = p0;
        for (int i = 1; i <= segmentsPerCurve; i++) {
            float t = (float)i / (float)segmentsPerCurve;
            Vector2 curr = evalCubic(p0, p1, p2, p3, t);
            DrawLineEx(prev, curr, 3.0f, curveColor);
            prev = curr;
        }
    }

    // --- Handles & knots ---
    for (int i = 0; i < (int)knots.size(); i++) {
        const SplineKnot& k = knots[i];
        bool isSel = (selection.knotIndex == i);

        // Handle lines
        DrawLineEx(k.handleIn,  k.position,  1.5f, handleLineColor);
        DrawLineEx(k.position,  k.handleOut, 1.5f, handleLineColor);

        // Handle-In dot
        bool hInActive = isSel && selection.type == SelectionType::HandleIn;
        DrawCircleV(k.handleIn, 5.0f, hInActive ? handleDotActive : handleDotDefault);

        // Handle-Out dot
        bool hOutActive = isSel && selection.type == SelectionType::HandleOut;
        DrawCircleV(k.handleOut, 5.0f, hOutActive ? handleDotActive : handleDotDefault);

        // Knot (outer ring + inner fill)
        bool knotActive = isSel && selection.type == SelectionType::Knot;
        DrawCircleV(k.position, 8.0f, knotActive ? knotActiveOuter : knotOuter);
        DrawCircleV(k.position, 5.0f, knotActive ? knotActiveInner : knotInner);

        // Label
        char label[16];
        std::snprintf(label, sizeof(label), "K%d", i);
        DrawText(label, (int)k.position.x + 12, (int)k.position.y - 8, 14, GRAY);
    }
}

// ─── ImGui Panel ────────────────────────────────────────────────────────────

void CubicBezierSpline::drawImGui() {
    ImGui::Begin("Spline Control");

    ImGui::Text("Cubic Bezier Spline (C1)");
    ImGui::Text("Knots: %d  |  Segments: %d", (int)knots.size(), std::max(0, (int)knots.size() - 1));
    ImGui::Separator();

    ImGui::SliderInt("Curve Smoothness", &segmentsPerCurve, 5, 100);
    ImGui::Separator();

    // ---- Selection info ----
    if (selection.type != SelectionType::None && selection.knotIndex >= 0
        && selection.knotIndex < (int)knots.size())
    {
        int idx = selection.knotIndex;
        const char* typeStr = "—";
        switch (selection.type) {
            case SelectionType::Knot:      typeStr = "Position";   break;
            case SelectionType::HandleIn:  typeStr = "Handle In";  break;
            case SelectionType::HandleOut: typeStr = "Handle Out"; break;
            default: break;
        }
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.3f, 1.0f), "Selected: Knot %d — %s", idx, typeStr);
        ImGui::Text("  Pos: (%.0f, %.0f)", knots[idx].position.x,  knots[idx].position.y);
        ImGui::Text("  In:  (%.0f, %.0f)", knots[idx].handleIn.x,  knots[idx].handleIn.y);
        ImGui::Text("  Out: (%.0f, %.0f)", knots[idx].handleOut.x, knots[idx].handleOut.y);
    } else {
        ImGui::TextDisabled("No selection — click a knot or handle");
    }

    ImGui::Separator();
    ImGui::Text("Edit:");

    if (ImGui::Button("Add Knot at End")) {
        addKnotAtEnd();
    }

    // Context-sensitive buttons when a knot is selected
    if (selection.type != SelectionType::None && selection.knotIndex >= 0) {
        // Insert only makes sense if there's a next knot (i.e., a segment to split)
        if (selection.knotIndex < (int)knots.size() - 1) {
            ImGui::SameLine();
            if (ImGui::Button("Insert After Selected")) {
                insertKnotAfter(selection.knotIndex);
            }
        }
        if ((int)knots.size() > 2) {
            if (ImGui::Button("Remove Selected Knot")) {
                removeKnot(selection.knotIndex);
            }
        }
    }

    ImGui::Spacing();
    if (ImGui::Button("Reset Spline")) {
        initDefaultKnots();
        selection  = { SelectionType::None, -1 };
        isDragging = false;
    }

    ImGui::Separator();
    ImGui::TextDisabled("Controls:");
    ImGui::TextDisabled("  Click & drag knots / handles");
    ImGui::TextDisabled("  Handles mirror through knot (C1)");
    ImGui::End();
}

// ─── Knot Manipulation ─────────────────────────────────────────────────────

void CubicBezierSpline::addKnot(Vector2 pos, Vector2 handleIn, Vector2 handleOut) {
    knots.push_back({ pos, handleIn, handleOut });
}

void CubicBezierSpline::addKnotAtEnd() {
    if (knots.empty()) {
        addKnot({ 640, 360 }, { 590, 360 }, { 690, 360 });
        return;
    }

    const SplineKnot& last = knots.back();
    constexpr float dx        = 120.0f;
    constexpr float handleLen = 40.0f;
    Vector2 newPos = { last.position.x + dx, last.position.y };
    addKnot(newPos,
            { newPos.x - handleLen, newPos.y },
            { newPos.x + handleLen, newPos.y });
}

void CubicBezierSpline::insertKnotAfter(int index) {
    if (index < 0 || index >= (int)knots.size() - 1) return;

    // Segment endpoints & handles
    Vector2 p0 = knots[index].position;
    Vector2 p1 = knots[index].handleOut;
    Vector2 p2 = knots[index + 1].handleIn;
    Vector2 p3 = knots[index + 1].position;

    // Position: curve midpoint
    Vector2 midPos = evalCubic(p0, p1, p2, p3, 0.5f);

    // Tangent at t=0.5 via Bézier derivative:
    //   B'(t) = 3[(1-t)^2(P1-P0) + 2(1-t)t(P2-P1) + t^2(P3-P2)]
    constexpr float t = 0.5f;
    constexpr float u = 0.5f;
    Vector2 tangent = {
        3.0f * (u * u * (p1.x - p0.x) + 2.0f * u * t * (p2.x - p1.x) + t * t * (p3.x - p2.x)),
        3.0f * (u * u * (p1.y - p0.y) + 2.0f * u * t * (p2.y - p1.y) + t * t * (p3.y - p2.y))
    };

    // Normalize, then scale to a reasonable handle length
    float len = std::sqrt(tangent.x * tangent.x + tangent.y * tangent.y);
    if (len > 0.001f) {
        constexpr float handleLen = 40.0f;
        tangent.x = tangent.x / len * handleLen;
        tangent.y = tangent.y / len * handleLen;
    }

    SplineKnot newKnot;
    newKnot.position  = midPos;
    newKnot.handleIn  = { midPos.x - tangent.x, midPos.y - tangent.y };
    newKnot.handleOut = { midPos.x + tangent.x, midPos.y + tangent.y };

    knots.insert(knots.begin() + index + 1, newKnot);
}

void CubicBezierSpline::removeKnot(int index) {
    if (index < 0 || index >= (int)knots.size()) return;
    if ((int)knots.size() <= 2) return;   // need at least 2 knots for 1 segment

    knots.erase(knots.begin() + index);
    selection  = { SelectionType::None, -1 };
    isDragging = false;
}