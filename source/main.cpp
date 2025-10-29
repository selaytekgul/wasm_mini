#include <iostream>
#include <imgui.h>
#include <rlImGui.h>

int main()
{
    using namespace raylib;
    InitWindow(800, 450, "raylib + ImGui + rlImGui (minimal)");
    SetTargetFPS(60);

    rlImGuiSetup(true); // true = dark theme

    bool show_demo = true;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // ImGui pass (between BeginDrawing/EndDrawing)
        rlImGuiBegin();
        ImGui::Begin("Hello"); ImGui::Text("Hi from ImGui + raylib!"); ImGui::End();
        if (show_demo) ImGui::ShowDemoWindow(&show_demo); // optional
        rlImGuiEnd();

        DrawText("Raylib draw still works under the ImGui overlay.", 10, 420, 10, DARKGRAY);
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}