#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "../external/raygui/src/raygui.h"

#include "pbt/python/python.h"
#include "pbt/core/log.h"


int main(void)
{
    pbt_log_set_level(0);
    pbt_log_trace("hello world");

    bool status = pbt_init_python();
    assert(status);

    InitWindow(1280, 720, "Programmable Blend Tree");

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 10.0f, 10.0f };  // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
                DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);

                DrawGrid(10, 1.0f);

            EndMode3D();

            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

            GuiMessageBox((Rectangle){ 85, 70, 250, 100 },
                    "#191#Message Box", "Hi! This is a message!", "Nice;Cool");

            DrawFPS(10, 10);

        EndDrawing();
    }

    CloseWindow();

    pbt_deinit_python();
    return 0;
}