#include "utils.h"
#include "GameController.hpp"

int main() {
    SetTraceLogLevel(LOG_NONE);
    // Initialization
    //--------------------------------------------------------------------------------------
#ifdef LOGGING
    spdlog::set_level(spdlog::level::debug);
#endif
    INFO("Starting window {0}*{1}", SCREEN_WIDTH, SCREEN_HEIGHT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
    INFO("Running at {0} fps", FPS);
    SetTargetFPS(FPS);
    GameController game("Game");
    float deltaTime;

    while (!WindowShouldClose())
    {
        //Update
        deltaTime = GetFrameTime();
        game.handleEvent();
        game.update(deltaTime);

        BeginDrawing();
            game.draw();
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
