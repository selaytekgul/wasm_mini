#include <iostream>
#include <imgui.h>
#include <rlImGui.h>
#include "Bezier.h"

int main()
{
    // using namespace raylib;
    raylib::InitWindow(800, 450, "raylib + ImGui + rlImGui (minimal)");
    raylib::SetTargetFPS(60);

    raylib::rlImGuiSetup(true); // true = dark theme

    bool show_demo = true;
    while (!raylib::WindowShouldClose()) {
        raylib::BeginDrawing();
        raylib::ClearBackground(raylib::RAYWHITE);

        // ImGui pass (between BeginDrawing/EndDrawing)
        raylib::rlImGuiBegin();
        ImGui::Begin("Hello"); ImGui::Text("Hi from ImGui + raylib!"); ImGui::End();
        if (show_demo) ImGui::ShowDemoWindow(&show_demo); // optional
        raylib::rlImGuiEnd();

        raylib::DrawText("Raylib draw still works under the ImGui overlay.", 10, 420, 10, raylib::DARKGRAY);
        raylib::EndDrawing();
    }

    raylib::rlImGuiShutdown();
    raylib::CloseWindow();
    return 0;
}