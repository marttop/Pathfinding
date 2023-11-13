/*
** File description:
** GameController
*/

#ifndef GAMECONTROLLER_HPP_
#define GAMECONTROLLER_HPP_

#include "utils.h"
#include "Grid.hpp"

class GameController : IScene {
public:
    GameController(const std::string& sceneName) : IScene(sceneName) {
        entities.push_back(std::make_shared<Grid>(Vector2{0.f, 0.f}, Rectangle{0.f, 0.f}));
    }

    ~GameController() = default;

    void update(float deltaTime) override;
    void handleEvent() override;
    void draw() const override;

protected:
private:
};

#endif /* !GAMECONTROLLER_HPP_ */
