#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "../external/raygui/src/raygui.h"

#include "pbt/python/python.h"
#include "pbt/python/pyskeleton.h"
#include "pbt/core/log.h"


int main(void)
{
    pbt_log_set_level(0);
    pbt_log_trace("hello world");

    bool status = pbt_init_python();
    assert(status);

    // load skeleton
    PbtSkeleton * skeleton = pbt_python_create_skeleton_from_file("data/character/AnimLabSkeleton.py");


    InitWindow(1280, 720, "Programmable Blend Tree");

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 150.0f, 150.0f, 150.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

    //DisableCursor();                    // Limit cursor to relative movement inside the window

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) UpdateCamera(&camera, CAMERA_FREE);
        if (IsKeyPressed('Z')) camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                for (uint32_t i=0 ; i<pbt_skeleton_bone_count(skeleton); ++i)
                {
                    Pbtfloat4 pos = pbt_skeleton_bone_pos(skeleton, i);
                    DrawSphere((Vector3){pos.values[0], pos.values[1], pos.values[2]}, 1, MAROON);

                }
                
                DrawGrid(10, 100.0f);

            EndMode3D();

            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

            GuiMessageBox((Rectangle){ 85, 70, 250, 100 },
                    "#191#Message Box", "Hi! This is a message!", "Nice;Cool");

            DrawFPS(10, 10);

        EndDrawing();
    }

    CloseWindow();

    // free skeleton
    pbt_skeleton_delete(skeleton);

    pbt_deinit_python();
    return 0;
}