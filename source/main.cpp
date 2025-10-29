// #include "raylib.h"
// #include "raymath.h"
#include "Bezier.h" // Your Bézier functions

#include "imgui.h"
#include "rlImGui.h" // The raylib-ImGui connector

/**
 * @brief Helper function to draw a control point with a selected state.
 */
using namespace raylib;
void DrawControlPoint(Vector2 pos, Color color, bool isSelected, bool isFrozen)
{
    if (isSelected)
    {
        // Draw a larger, bright circle to show it's selected
        DrawCircleV(pos, 10, WHITE);

        if (isFrozen)
        {
            // Draw a "hollow" circle to show it's frozen
            DrawCircleV(pos, 8, BLACK);
            DrawCircleV(pos, 6, color);
        }
        else
        {
            // Draw a "solid" circle to show it's active
            DrawCircleV(pos, 8, color);
        }
    }
    else
    {
        DrawCircleV(pos, 7, color);
    }
}

/**
 * ===================================================================================
 * MAIN APPLICATION
 * ===================================================================================
 */
int main(void)
{
    Bezier b;
    // --- Setup ---
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Interactive Bézier Demo (Click-to-Freeze)");
    SetTargetFPS(60);

    // --- ImGui Setup ---
    rlImGuiSetup(true);

    // --- Control Points for Quadratic Curve ---
    Vector2 q_p0 = { 100, (float)screenHeight / 2 + 100 };
    Vector2 q_p1 = { 250, 100 };
    Vector2 q_p2 = { 400, (float)screenHeight / 2 + 100 };

    // --- Control Points for Cubic Curve ---
    Vector2 c_p0 = { 600, (float)screenHeight / 2 };
    Vector2 c_p1 = { 700, 100 };
    Vector2 c_p2 = { 900, (float)screenHeight - 100 };
    Vector2 c_p3 = { 1100, (float)screenHeight / 2 };

    // --- STATE MANAGEMENT ---

    // 1. Which point is selected (via ImGui)
    int selectedPoint = 0; // 0 = None, 1-3 = Quad, 4-7 = Cubic

    // 2. NEW: Is the point "frozen" or "active" (following the mouse)?
    //    We start as 'true' so the point isn't moving on launch.
    bool isMovementFrozen = true;


    // --- Main Game Loop ---
    while (!WindowShouldClose())
    {
        // --- Update ---
        Vector2 mousePos = GetMousePosition();
        ImGuiIO& io = ImGui::GetIO();

        // **CRITICAL LOGIC BLOCK**
        if (!io.WantCaptureMouse)
        {
            // The mouse is in the main window, not over ImGui

            // 1. Check for the "toggle" click
            if (IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT))
            {
                isMovementFrozen = !isMovementFrozen; // Flip the state
            }

            // 2. Only move the point IF movement is NOT frozen
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
                }
            }
        }

        // --- Draw ---
        BeginDrawing();
        ClearBackground(BLACK);

        // --- Draw Quadratic Example ---
        DrawText("Quadratic Bézier", 100, 50, 20, GRAY);
        b.drawQuadraticCurve(q_p0, q_p1, q_p2, 3.0f, WHITE, 30);
        DrawLineV(q_p0, q_p1, GRAY);
        DrawLineV(q_p1, q_p2, GRAY);

        // Pass the 'isMovementFrozen' state to the draw helper
        DrawControlPoint(q_p0, RED,   selectedPoint == 1, isMovementFrozen);
        DrawControlPoint(q_p1, GREEN, selectedPoint == 2, isMovementFrozen);
        DrawControlPoint(q_p2, RED,   selectedPoint == 3, isMovementFrozen);

        // --- Draw Cubic Example ---
        DrawText("Cubic Bézier", 600, 50, 20, GRAY);
        b.drawCubicCurve(c_p0, c_p1, c_p2, c_p3, 3.0f, WHITE, 40);
        DrawLineV(c_p0, c_p1, GRAY);
        DrawLineV(c_p1, c_p2, GRAY);
        DrawLineV(c_p2, c_p3, GRAY);

        DrawControlPoint(c_p0, BLUE,   selectedPoint == 4, isMovementFrozen);
        DrawControlPoint(c_p1, YELLOW, selectedPoint == 5, isMovementFrozen);
        DrawControlPoint(c_p2, YELLOW, selectedPoint == 6, isMovementFrozen);
        DrawControlPoint(c_p3, BLUE,   selectedPoint == 7, isMovementFrozen);

        // --- IMGUI UI DRAW ---
        rlImGuiBegin();

        ImGui::Begin("Control Panel");

        // NEW: Add a status indicator so the user knows the state
        ImGui::Text("Movement Status:");
        if (isMovementFrozen)
        {
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "FROZEN");
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

        // Radio buttons now just select the point. They don't affect movement.
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

        ImGui::End();

        rlImGuiEnd();

        EndDrawing();
    }

    // --- Cleanup ---
    rlImGuiShutdown();
    CloseWindow();
    return 0;
}