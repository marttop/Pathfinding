/*
** File description:
** GameController
*/

#include "GameController.hpp"

void GameController::update(float deltaTime)
{
    for (const auto &e : entities) {
        e->update(deltaTime, entities);
    }
}

void GameController::handleEvent()
{
}

void GameController::draw() const
{
    ClearBackground(BLUE);
    for (const auto &e : entities) {
        e->draw();
        e->drawGUI();
    }
}
