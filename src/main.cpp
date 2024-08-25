#include "raylib.h"
#include "game.hpp"
#include "constants.hpp"

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Word Association Game by Kevin");
    SetTargetFPS(60);
    
    Game game;

    while (!WindowShouldClose()) {
        game.Update();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        game.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}