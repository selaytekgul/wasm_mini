#include "CubicBezierSpline.h"
#include "Bezier.h"
#include "imgui.h"
#include "rlImGui.h"
#include <cstdio>

using namespace raylib;

void DrawControlPoint(Vector2 pos, Color color, bool isSelected, bool isFrozen)
{
    if (isSelected)
    {
        DrawCircleV(pos, 10, WHITE);
        if (isFrozen) { DrawCircleV(pos, 8, BLACK); DrawCircleV(pos, 6, color); }
        else          { DrawCircleV(pos, 8, color); }
    }
    else { DrawCircleV(pos, 7, color); }
}

void runOriginalBezierDemo()
{
    Bezier b;
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Bézier");
    SetTargetFPS(60);
    rlImGuiSetup(true);

    Vector2 q_p0 = { 50, (float)screenHeight / 2 + 100 };
    Vector2 q_p1 = { 150, 100 };
    Vector2 q_p2 = { 200, (float)screenHeight / 2 + 100 };
    Vector2 c_p0 = { 250, (float)screenHeight / 2 };
    Vector2 c_p1 = { 300, 100 };
    Vector2 c_p2 = { 450, (float)screenHeight - 100 };
    Vector2 c_p3 = { 500, (float)screenHeight / 2 };
    Vector2 d4_p0 = { 550, (float)screenHeight / 2 };
    Vector2 d4_p1 = { 600, 100 };
    Vector2 d4_p2 = { 650, (float)screenHeight - 100 };
    Vector2 d4_p3 = { 700, (float)screenHeight / 2 };
    Vector2 d4_p4 = { 750, (float)screenHeight - 100 };
    Vector2 d5_p0 = { 800, (float)screenHeight / 2 };
    Vector2 d5_p1 = { 850, 100 };
    Vector2 d5_p2 = { 900, (float)screenHeight - 100 };
    Vector2 d5_p3 = { 950, (float)screenHeight / 2 };
    Vector2 d5_p4 = { 1000, (float)screenHeight - 100 };
    Vector2 d5_p5 = { 1050, (float)screenHeight - 100 };

    int selectedPoint = 0;
    bool isMovementFrozen = true;
    int quadraticSlider = 1;
    int quadraticSegments = 10;
    int cubicSegments = 10;
    int degree4Segments = 10;
    int degree5Segments = 10;

    while (!WindowShouldClose())
    {
        Vector2 mousePos = GetMousePosition();
        ImGuiIO& io = ImGui::GetIO();
        if (!io.WantCaptureMouse)
        {
            if (IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT))
                isMovementFrozen = !isMovementFrozen;

            if (!isMovementFrozen)
            {
                switch (selectedPoint)
                {
                    case 1: q_p0 = mousePos; break;
                    case 2: q_p1 = mousePos; break;
                    case 3: q_p2 = mousePos; break;
                    case 4: c_p0 = mousePos; break;
                    case 5: c_p1 = mousePos; break;
                    case 6: c_p2 = mousePos; break;
                    case 7: c_p3 = mousePos; break;
                    case 8: d4_p0 = mousePos; break;
                    case 9: d4_p1 = mousePos; break;
                    case 10: d4_p2 = mousePos; break;
                    case 11: d4_p3 = mousePos; break;
                    case 12: d4_p4 = mousePos; break;
                    case 13: d5_p0 = mousePos; break;
                    case 14: d5_p1 = mousePos; break;
                    case 15: d5_p2 = mousePos; break;
                    case 16: d5_p3 = mousePos; break;
                    case 17: d5_p4 = mousePos; break;
                    case 18: d5_p5 = mousePos; break;
                }
            }
        }
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Quadratic Bézier", 50, 50, 20, GRAY);
        b.drawQuadraticCurveUsingSlider(q_p0, q_p1, q_p2, 3.0f, WHITE, quadraticSegments, quadraticSlider);
        DrawLineV(q_p0, q_p1, GRAY);
        DrawLineV(q_p1, q_p2, GRAY);
        DrawControlPoint(q_p0, RED,   selectedPoint == 1, isMovementFrozen);
        DrawControlPoint(q_p1, GREEN, selectedPoint == 2, isMovementFrozen);
        DrawControlPoint(q_p2, RED,   selectedPoint == 3, isMovementFrozen);

        DrawText("Cubic Bézier", 350, 50, 20, GRAY);
        b.drawCubicCurve(c_p0, c_p1, c_p2, c_p3, 3.0f, WHITE, cubicSegments);
        DrawLineV(c_p0, c_p1, GRAY);
        DrawLineV(c_p1, c_p2, GRAY);
        DrawLineV(c_p2, c_p3, GRAY);
        DrawControlPoint(c_p0, BLUE,   selectedPoint == 4, isMovementFrozen);
        DrawControlPoint(c_p1, YELLOW, selectedPoint == 5, isMovementFrozen);
        DrawControlPoint(c_p2, YELLOW, selectedPoint == 6, isMovementFrozen);
        DrawControlPoint(c_p3, BLUE,   selectedPoint == 7, isMovementFrozen);

        DrawText("Degree 4 Bézier", 700, 50, 20, GRAY);
        b.drawDegree4Curve(d4_p0, d4_p1, d4_p2, d4_p3, d4_p4, 3.0f, WHITE, degree4Segments);
        DrawLineV(d4_p0, d4_p1, GRAY);
        DrawLineV(d4_p1, d4_p2, GRAY);
        DrawLineV(d4_p2, d4_p3, GRAY);
        DrawLineV(d4_p3, d4_p4, GRAY);
        DrawControlPoint(d4_p0, BLUE,   selectedPoint == 8, isMovementFrozen);
        DrawControlPoint(d4_p1, YELLOW, selectedPoint == 9, isMovementFrozen);
        DrawControlPoint(d4_p2, YELLOW, selectedPoint == 10, isMovementFrozen);
        DrawControlPoint(d4_p3, YELLOW, selectedPoint == 11, isMovementFrozen);
        DrawControlPoint(d4_p4, BLUE,   selectedPoint == 12, isMovementFrozen);

        DrawText("Degree 5 Bézier", 950, 50, 20, GRAY);
        b.drawDegree5Curve(d5_p0, d5_p1, d5_p2, d5_p3, d5_p4, d5_p5, 3.0f, WHITE, degree5Segments);
        DrawLineV(d5_p0, d5_p1, GRAY);
        DrawLineV(d5_p1, d5_p2, GRAY);
        DrawLineV(d5_p2, d5_p3, GRAY);
        DrawLineV(d5_p3, d5_p4, GRAY);
        DrawLineV(d5_p4, d5_p5, GRAY);
        DrawControlPoint(d5_p0, BLUE,  selectedPoint == 13, isMovementFrozen);
        DrawControlPoint(d5_p1, GREEN, selectedPoint == 14, isMovementFrozen);
        DrawControlPoint(d5_p2, GREEN, selectedPoint == 15, isMovementFrozen);
        DrawControlPoint(d5_p3, GREEN, selectedPoint == 16, isMovementFrozen);
        DrawControlPoint(d5_p4, GREEN, selectedPoint == 17, isMovementFrozen);
        DrawControlPoint(d5_p5, BLUE,  selectedPoint == 18, isMovementFrozen);

        rlImGuiBegin();
        ImGui::Begin("Control Panel");
        ImGui::Text("Movement Status:");
        if (isMovementFrozen)
        {
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "FROZEN!");
            ImGui::Text("Click in the window to unfreeze.");
        }
        else
        {
            ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "ACTIVE");
            ImGui::Text("Click in the window to freeze.");
        }
        ImGui::Separator();
        ImGui::Text("Select Point to Move:");
        ImGui::Separator();
        ImGui::SliderInt("Quadratic Slider", &quadraticSlider, 1, 20);
        ImGui::Separator();
        ImGui::RadioButton("None", &selectedPoint, 0);
        ImGui::Separator();
        ImGui::Text("Quadratic Curve");
        ImGui::RadioButton("Q Start (P0)", &selectedPoint, 1);
        ImGui::RadioButton("Q Control (P1)", &selectedPoint, 2);
        ImGui::RadioButton("Q End (P2)", &selectedPoint, 3);
        ImGui::Separator();
        ImGui::Text("Cubic Curve");
        ImGui::RadioButton("C Start (P0)", &selectedPoint, 4);
        ImGui::RadioButton("C Control (P1)", &selectedPoint, 5);
        ImGui::RadioButton("C Control (P2)", &selectedPoint, 6);
        ImGui::RadioButton("C End (P3)", &selectedPoint, 7);
        ImGui::Separator();
        ImGui::Text("Degree 4 Curve");
        ImGui::RadioButton("D4 Start (P0)", &selectedPoint, 8);
        ImGui::RadioButton("D4 Control (P1)", &selectedPoint, 9);
        ImGui::RadioButton("D4 Control (P2)", &selectedPoint, 10);
        ImGui::RadioButton("D4 Control (P3)", &selectedPoint, 11);
        ImGui::RadioButton("D4 End (P4)", &selectedPoint, 12);
        ImGui::Separator();
        ImGui::Text("Degree 5 Curve");
        ImGui::RadioButton("D5 Start (P0)", &selectedPoint, 13);
        ImGui::RadioButton("D5 Control (P1)", &selectedPoint, 14);
        ImGui::RadioButton("D5 Control (P2)", &selectedPoint, 15);
        ImGui::RadioButton("D5 Control (P3)", &selectedPoint, 16);
        ImGui::RadioButton("D5 Control (P4)", &selectedPoint, 17);
        ImGui::RadioButton("D5 End (P5)", &selectedPoint, 18);
        ImGui::Separator();
        ImGui::Text("Curve Smoothness:");
        ImGui::SliderInt("Quadratic Segments", &quadraticSegments, 1, 20);
        ImGui::SliderInt("Cubic Segments", &cubicSegments, 1, 20);
        ImGui::SliderInt("Degree 4 Segments", &degree4Segments, 1, 20);
        ImGui::SliderInt("Degree 5 Segments", &degree5Segments, 1, 20);
        ImGui::End();
        rlImGuiEnd();
        EndDrawing();
    }
    rlImGuiShutdown();
    CloseWindow();
}

// ═══════════════════════════════════════════════════════════════════

int main(void)
{
    // runOriginalBezierDemo(); return 0;

    const int W = 1280, H = 720;
    InitWindow(W, H, "Cubic Bezier Spline - C1");
    SetTargetFPS(60);
    rlImGuiSetup(true);

    CubicBezierSpline spline;
    float margin = 150.0f, spacing = (W - 2 * margin) / 4.0f;
    for (int i = 0; i < 5; i++)
    {
        float x = margin + i * spacing;
        float y = H / 2.0f + ((i % 2 == 0) ? -100.0f : 100.0f);
        spline.knots.push_back({ {x, y}, {x - 40, y}, {x + 40, y} });
    }

    int selKnot = -1, selPart = 0; // 0=pos, 1=hIn, 2=hOut
    bool frozen = true;
    int segments = 40;

    while (!WindowShouldClose())
    {
        Vector2 mouse = GetMousePosition();
        ImGuiIO& io = ImGui::GetIO();

        if (!io.WantCaptureMouse)
        {
            if (IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT))
                frozen = !frozen;

            if (!frozen && selKnot >= 0 && selKnot < (int)spline.knots.size())
            {
                SplineKnot& k = spline.knots[selKnot];
                if (selPart == 0)      k.pos = mouse;
                else if (selPart == 1) { k.hIn = mouse;  spline.enforceC1(selKnot, false); }
                else                   { k.hOut = mouse; spline.enforceC1(selKnot, true);  }
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        spline.drawSpline(segments);

        for (int i = 0; i < (int)spline.knots.size(); i++)
        {
            SplineKnot& k = spline.knots[i];
            bool sel = (selKnot == i);
            DrawLineV(k.hIn, k.pos, GRAY);
            DrawLineV(k.pos, k.hOut, GRAY);
            DrawCircleV(k.hIn,  5, (sel && selPart == 1) ? YELLOW : GRAY);
            DrawCircleV(k.hOut, 5, (sel && selPart == 2) ? YELLOW : GRAY);
            DrawCircleV(k.pos,  7, (sel && selPart == 0) ? YELLOW : RED);
        }

        rlImGuiBegin();
        ImGui::Begin("Spline Control");

        if (frozen) { ImGui::TextColored({1,0.4f,0.4f,1}, "FROZEN"); }
        else        { ImGui::TextColored({0.4f,1,0.4f,1}, "ACTIVE"); }

        ImGui::SliderInt("Segments", &segments, 5, 100);
        ImGui::Separator();

        int enc = (selKnot < 0) ? -1 : selKnot * 3 + selPart;
        ImGui::RadioButton("None", &enc, -1);
        for (int i = 0; i < (int)spline.knots.size(); i++)
        {
            ImGui::Separator();
            char l[32];
            std::snprintf(l, 32, "K%d Pos", i);         ImGui::RadioButton(l, &enc, i * 3);
            std::snprintf(l, 32, "K%d Handle In", i);   ImGui::RadioButton(l, &enc, i * 3 + 1);
            std::snprintf(l, 32, "K%d Handle Out", i);  ImGui::RadioButton(l, &enc, i * 3 + 2);
        }
        if (enc < 0) { selKnot = -1; selPart = 0; }
        else         { selKnot = enc / 3; selPart = enc % 3; }

        ImGui::End();
        rlImGuiEnd();
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}