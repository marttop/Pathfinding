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

    //INIT GUI
    //Init GUI
    Color windowColor = (Color){ 252, 255, 59, 170 }; // Semi-transparent yellow
    Color hoverColor = (Color){ 80, 80, 80, 170 }; // Semi-transparent dark grey

    // GuiSetStyle(STATUSBAR, BASE_COLOR_NORMAL, ColorToInt(YELLOW));
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ColorToInt(windowColor));
    GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, ColorToInt(BLACK));
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
    GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
    GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, ColorToInt(hoverColor));

    GuiSetStyle(CHECKBOX, BORDER_COLOR_NORMAL, ColorToInt(BLACK));
    GuiSetStyle(CHECKBOX, BORDER_COLOR_NORMAL, ColorToInt(BLACK));
    GuiSetStyle(CHECKBOX, BORDER_COLOR_FOCUSED, ColorToInt(WHITE));

    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, ColorToInt(BLACK));
    // GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(YELLOW));
    GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, ColorToInt(WHITE));
    GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));

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
