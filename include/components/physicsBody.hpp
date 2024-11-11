#pragma once

#include "Component.hpp"
#include "Entity.hpp"
#include "Math.hpp"
#include "SolidBody.hpp"

/**
 * \brief A simple movement physics class
 * Made mainly for players
 * It uses a Vector2f velocity and the entityBox slide method
 */
class physicsBody : public Component {
public:
  physicsBody(Entity &entity) : Component(entity) {}
  void update() override;

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

