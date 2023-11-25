#ifndef COMPONENT_H
#define COMPONENT_H

#include <iostream>

// #include "Entity.h"
// #include "EntityBox.h"

class Scene;
struct entityBox;
class Entity;

/**
 * \brief Components are made to attach to Entity s Sprite Components with
 * handle rendering while Collider s will handle collisions
 */
class Component {
public:
  Component(std::string _title = "") : title(_title){};
  virtual ~Component() {}
  /**
   * \brief Runs after Component data is set in entity.add
   */
  virtual void start() = 0;
  /**
   * \brief Updates in the update loop
   */
  virtual void update(float deltaTime) = 0;
  /**
   * \brief Is called when entity->toDestory = true
   */
  virtual void onDestroy() {}
  /**
   * \brief Runs when the screen size is changed
   */
  virtual void onScreenChange() {}

  entityBox *box;
  /**
   * \brief The attached entity
   */
  Entity *entity = nullptr;
  Scene *scene;
  std::string title = "";
  bool isInit = false;
  /**
   * \brief Layer to update on (requires useLayer=true)
   */
  int layer = 0;
  int index;
  std::string entityTag = "";

  /**
   * \brief Tells the engine to update with a sorting algorithim (made for sprites)
   */
  bool useLayer = false;
  /**
   * \brief Will update the Component in the game loop
   */
  bool standardUpdate = true;
};

#endif
