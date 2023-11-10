// IController.hpp

#ifndef ICONTROLLER_HPP_
#define ICONTROLLER_HPP_

#include "IEntity.hpp"

class IController {
public:
    /**
     * @brief Default destructor.
     */
    virtual ~IController() = default;

    /**
     * @brief Adds an entity to the scene.
     * @param entity The entity to add to the scene.
     */
    virtual void addEntity(std::shared_ptr<IEntity> entity) = 0;

    /**
     * @brief Removes an entity from the scene.
     * @param entity The entity to remove from the scene.
     */
    virtual void removeEntity(std::shared_ptr<IEntity> entity) = 0;

    /**
     * @brief Updates all entities in the scene.
     * @param deltaTime The time passed since the last update.
     */
    virtual void update(float deltaTime) = 0;

    /**
     * @brief Gets the entity's bounding rectangle.
     *
     * This function returns the bounding rectangle of the entity if it exists.
     * If the entity doesn't have a bounding rectangle, it returns an empty or invalid rectangle.
     * The default implementation returns an empty rectangle.
     *
     * @return The entity's bounding rectangle or an empty/invalid rectangle if not applicable.
     */
    virtual Rectangle getRect() const
    {
        return {0, 0, 0, 0};
    }

    /**
     * @brief Handles events in the scene.
     */
    virtual void handleEvent() = 0;

    /**
     * @brief Draws all entities in the scene.
     */
    virtual void draw() const = 0;
};

#endif /* !ICONTROLLER_HPP_ */

class IScene : public IController {
private:
    std::string _sceneName;
protected:
    std::vector<std::shared_ptr<IEntity>> entities;

public:
    IScene(const std::string& sceneName) : _sceneName(sceneName) {
        INFO("Init scene : {0}", sceneName);
    }
    virtual ~IScene() = 0;

    void addEntity(std::shared_ptr<IEntity> entity) override {
        entities.push_back(entity);
    }

    void removeEntity(std::shared_ptr<IEntity> entity) override {
        entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
    }

    void update(float deltaTime) override {
        for (const auto& entity : entities) {
            entity->update(deltaTime, entities);
        }
    }

    virtual void draw() const override {
        for (const auto& entity : entities) {
            entity->draw();
        }
    }

    virtual void handleEvent() override = 0;
};

inline IScene::~IScene() {}