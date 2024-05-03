#pragma once

#include "Collider.h"
#include "Component.h"
#include "Entity.h"
#include "Math.h"
#include "Serializer.h"

/**
 * \brief A simple movement physics class
 * Made mainly for players
 * It uses a Vector2f velocity and the entityBox slide method
 */
class physicsBody : public Component {
public:
  physicsBody() : Component("physicsBody"){};

  void start() override { entity->require<Collider>(); }

  void update(float deltaTime) override {
    // Adjusts velocity if greater then max (Ignores if -1)
    if (maxVelocity.x != -1) {
      if (abs(velocity.x) > abs(maxVelocity.x))
        velocity.x = velocity.x / abs(velocity.x) * maxVelocity.x;
    }
    if (maxVelocity.y != -1) {
      if (abs(velocity.y) > abs(maxVelocity.y))
        velocity.y = velocity.y / abs(velocity.y) * maxVelocity.y;
    }

    // Prevents teleporting if people click of the window and
    // make deltatime bigger than... idk something problably
    if (deltaTime > 0.2)
      std::cout << deltaTime << std::endl;
    if (deltaTime > 0.2)
      return;

    // Check for pointless move
    if (velocity.x == 0 && velocity.y == 0)
      return;

    std::vector<Entity *> solids;
    for (Collider *col : GameManager::getComponents<Collider>()) {
      if (!col->solid)
        continue;
      solids.push_back(col->entity);
    }

    if (pushOut) {
      slideOut out = entity->box.slide(velocity * deltaTime, solids);

      if (out.horizontalHit)
        velocity.x = 0;
      if (out.verticleHit)
        velocity.y = 0;
    } else {
      entity->box.position += (velocity * deltaTime);
    }
  }

  /**
   * \brief The velocity for this body
   */
  Vector2f velocity;
  /**
   * \brief Prevents the velocity from getting to big
   *
   * This works with abs so it doens't matter for negtive and positive values
   * Default its set to -1, -1 (Which tells the body to ignore it and let
   * velocity go any amount)
   */
  Vector2f maxVelocity = {-1, -1};
  /**
   * \brief If false will ignore solid colliders and pass through
   */
  bool pushOut = true;
};

REGISTER_COMPONENT_TYPE(physicsBody);
