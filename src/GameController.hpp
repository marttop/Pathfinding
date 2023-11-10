/*
** File description:
** GameController
*/

#ifndef GAMECONTROLLER_HPP_
#define GAMECONTROLLER_HPP_

#include "utils.h"
#include "IController.hpp"

class GameController : IScene {
public:
    GameController(const std::string& sceneName) : IScene(sceneName) {}

    ~GameController() = default;

    void update(float deltaTime) override;
    void handleEvent() override;
    void draw() const override;

protected:
private:
};

#endif /* !GAMECONTROLLER_HPP_ */
