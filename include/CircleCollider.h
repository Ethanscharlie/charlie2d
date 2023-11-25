#pragma once

#include "Component.h"
#include "Entity.h"
#include "EntityBox.h"
#include "GameManager.h"
#include "Math.h"
#include <SDL.h>

typedef std::vector<Component *> Group;

/**
 * \brief For all your ball touching needs!
 */
class CircleCollider : public Component {
public:
  CircleCollider() : Component("CircleCollider"){};

  /**
   * \brief Sets the radius to the height/2 ig
   */
  void start() override { radius = entity->box->getSize().y / 2; }

  /**
   * \brief Keeps sets the radius? Probably needs to be fixed
   */
  void update(float deltaTime) override {
    radius = entity->box->getSize().y / 2;
  }

  /**
   * \brief Gets all active collisions with other CircleCollider s
   * \param tag allowes you to narrow collisions down to only with a certain tag
   */
  std::vector<CircleCollider *> getCollisions(std::string tag = "") {
    radius = entity->box->getSize().y / 2;
    std::vector<CircleCollider *> hits;

    for (auto *col : entity->scene->getGroup<CircleCollider>(tag)) {
      if (col->entity == entity)
        continue;
      if (checkCircleCollision(col)) {
        hits.push_back(col);
      }
    }

    return hits;
  }

  /**
   * \brief Checks a collision with one other CircleCollider... again?
   */
  bool checkCollision(CircleCollider *col) { return checkCircleCollision(col); }

  /**
   * \brief Checks a collision with one other CircleCollider
   */
  bool checkCircleCollision(CircleCollider *col) {
    float distance =
        std::sqrt(std::pow(col->entity->box->getBox().getCenter().x -
                               entity->box->getBox().getCenter().x,
                           2) +
                  std::pow(col->entity->box->getBox().getCenter().y -
                               entity->box->getBox().getCenter().y,
                           2));

    // bool c = distance <= radius + col->radius;
    // std::cout << c << std::endl;
    return distance <= radius + col->radius;
  }

  float radius = 0;
  /**
   * \brief Is solid for physics systems... which don't exit for circles yet
   * :sadpepe:
   */
  bool solid = false;
};
