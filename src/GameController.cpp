/*
** File description:
** GameController
*/

#include "GameController.hpp"

void GameController::update(float deltaTime)
{
    for (auto e : entities) {
        e->update(deltaTime, entities);
    }
}

void GameController::handleEvent()
{
}

void GameController::draw() const
{
    ClearBackground(BLUE);
    for (auto e : entities) {
        e->draw();
    }
}