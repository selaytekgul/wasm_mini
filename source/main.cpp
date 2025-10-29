// #include "raylib.h"
// #include "raymath.h"
#include "Bezier.h"

using namespace raylib;

int main(void)
{
    Bezier b;
    // --- Setup ---
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Bézier Curve Math Demo");
    SetTargetFPS(60);

    // --- Control Points for Quadratic Curve ---
    Vector2 q_p0 = { 100, (float)screenHeight / 2 };
    Vector2 q_p1 = { 0, 0 }; // This one will follow the mouse
    Vector2 q_p2 = { 400, (float)screenHeight / 2 };

    // --- Control Points for Cubic Curve ---
    Vector2 c_p0 = { 600, (float)screenHeight / 2 };
    Vector2 c_p1 = { 700, 100 };
    Vector2 c_p2 = { 900, (float)screenHeight - 100 };
    Vector2 c_p3 = { 1100, (float)screenHeight / 2 };


    // --- Main Game Loop ---
    while (!WindowShouldClose())
    {
        // --- Update ---
        // Make the quadratic control point follow the mouse
        q_p1 = GetMousePosition();

        // --- Draw ---
        BeginDrawing();
        ClearBackground(BLACK);

        // --- Draw Quadratic Example ---
        DrawText("Quadratic Bézier", 100, 50, 20, GRAY);

        // 1. Draw the smooth curve
        b.drawQuadraticCurve(q_p0, q_p1, q_p2, 3.0f, WHITE, 30);

        // 2. Draw the control "legs"
        DrawLineV(q_p0, q_p1, GRAY);
        DrawLineV(q_p1, q_p2, GRAY);

        // 3. Draw the control points
        DrawCircleV(q_p0, 7, RED);
        DrawCircleV(q_p1, 7, GREEN); // The one you move
        DrawCircleV(q_p2, 7, RED);


        // --- Draw Cubic Example ---
        DrawText("Cubic Bézier", 600, 50, 20, GRAY);

        // 1. Draw the smooth curve
        b.drawCubicCurve(c_p0, c_p1, c_p2, c_p3, 3.0f, WHITE, 40);

        // 2. Draw the control "legs"
        DrawLineV(c_p0, c_p1, GRAY);
        DrawLineV(c_p1, c_p2, GRAY);
        DrawLineV(c_p2, c_p3, GRAY);

        // 3. Draw the control points
        DrawCircleV(c_p0, 7, BLUE);
        DrawCircleV(c_p1, 7, YELLOW);
        DrawCircleV(c_p2, 7, YELLOW);
        DrawCircleV(c_p3, 7, BLUE);


        DrawText("Move mouse to control the green point", 10, 10, 20, LIGHTGRAY);
        DrawFPS(screenWidth - 100, 10);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}