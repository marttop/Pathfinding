// IEntity.hpp
#ifndef IENTITY_HPP_
#define IENTITY_HPP_

#include "utils.h"

enum class EntityType {
    Entity,
};

/**
 * @interface IEntity
 * @brief An interface for game entities.
 */
class IEntity {
public:
    /**
     * @brief Default destructor.
     */
    virtual ~IEntity() = default;

    /**
     * @brief Draws the entity on the screen.
     */
    virtual void draw() const = 0;

    /**
     * @brief Gets the entity's position as a Vector2.
     * @return The entity's position as a Vector2.
     */
    virtual Vector2 getPosition() const = 0;

    /**
     * @brief Gets the entity's type.
     * @return The entity's EntityType.
     */
    virtual EntityType getEntityType() const = 0;

    /**
     * @brief Sets the entity's position with a Vector2.
     * @param position The position to set the entity to.
     */
    virtual void setPosition(const Vector2& position) = 0;

    /**
     * @brief Gets the entity's bounding rectangle.
     *
     * This function returns the bounding rectangle of the entity if it exists.
     * If the entity doesn't have a bounding rectangle, it returns an empty or invalid rectangle.
     * The default implementation returns an empty rectangle.
     *
     * @return The entity's bounding rectangle or an empty/invalid rectangle if not applicable.
     */
    virtual Rectangle getRect() const {
        return {0, 0, 0, 0};
    }

    /**
     * @brief Updates the entity.
     * @param deltaTime The time passed since the last update.
     * @param m_entities A vector of shared pointers to other entities in the game.
     */
    virtual void update(float deltaTime, std::vector<std::shared_ptr<IEntity>>& m_entities) = 0;

};

#endif /* !IENTITY_HPP_ */