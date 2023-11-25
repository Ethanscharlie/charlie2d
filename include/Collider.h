#ifndef COLLIDER_H
#define COLLIDER_H

#include "Component.h"
#include "EntityBox.h"
#include "GameManager.h"
#include "Math.h"
#include <SDL.h>
#include <iostream>

typedef std::vector<Component *> Group;

/**
 * \brief Colliders event tho Box.checkCollision exists too
 */
class Collider : public Component {
public:
  Collider() : Component("collider"){};

  void start() override;
  void update(float deltaTime) override;

  /**
   * \brief Gets all active collisions with other Collider s
   * \param tag allowes you to narrow collisions down to only with a certain tag
   */
  std::vector<Collider *> getCollisions(std::string tag = "");
  /**
   * \brief Checks a collision with one other Collider... again?
   */
  bool checkCollision(Collider col);
  /**
   * \brief Checks a collision with one other Collider
   */
  bool checkBoxCollision(Box otherBox);

  Box colliderBox;
  /**
   * \brief Is solid for physics systems
   */
  bool solid = false;
};

#endif
