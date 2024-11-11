#ifndef ENTITY_H
#define ENTITY_H

#include "Box.hpp"
#include "Math.hpp"
#include <SDL.h>
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <typeindex>
#include <vector>

class Animation;
class Component;

/**
 * \brief Entitys are containers for an EnityBox and components
 */
class Entity {
public:
  Entity() {}

  ~Entity() {}

  /**
   * \brief Adds a component to the entity
   */
  template <typename C> C &addComponent();
  /**
   * \brief Gets a component from the entity
   */
  template <typename C> C &getComponent();

  /**
   * \brief Removes and deletes a component
   */
  template <typename C> void remove();
  /**
   * \brief Does the enity have this? Ex: checkComponent<Sprite>()
   */
  template <typename C> bool checkComponent();

  /**
   * \brief Removes and deletes a component via type index
   */
  void remove(std::type_index type);

  bool isQueuedForDestruction();

  /**
   * \brief Updates all components, is called my GameManager
   */
  void update();

  /**
   * \brief String tag for the entity
   */
  std::string tag = "";

  /**
   * \brief Marks the entity for deletion
   */
  void destroy();

  /**
   * \brief Tells the game to render the entity either in the world or on the
   * screen. Screen is meant for UI
   */
  EntityRenderPositionType renderPositionType = EntityRenderPositionType::World;

  /**
   * \brief Unique int id
   */
  int iid;
  /**
   * \brief Skips the update loop for the next frame (Resets to true after)
   */
  bool skipUpdate = false;
  /**
   * \brief NO MORE UPDATES
   */
  bool active = true;
  /**
   * \brief Layer to update on (requires entity->useLayer=true)
   */
  int layer = 0;

  /**
   *\brief A pointer to the entitys entityBox
   */
  Box box;

private:
  std::map<std::type_index, std::unique_ptr<Component>> components;

  /**
   * \brief If marked true the entity and all its components will be removed and
   * deleted from memory
   */
  bool toDestroy = false;
};

//
//
//
//
//
//
//

template <typename C> C &Entity::addComponent() {
  if (checkComponent<C>()) {
    return getComponent<C>();
  }

  std::unique_ptr<C> component = std::make_unique<C>(*this);
  Component* componentPtr = component.get();
  components[typeid(C)] = std::move(component);

  return *static_cast<C *>(componentPtr);
}

template <typename C> C &Entity::getComponent() {
  auto it = components.find(typeid(C));
  if (it != components.end()) {
    if (it->second == nullptr)
      throw std::runtime_error("Component was nullptr");

    return *static_cast<C *>(it->second.get());
  }

  else {
    throw std::runtime_error("Component not found!");
  }
}

template <typename C> void Entity::remove() {
  C *component = getComponent<C>();
  components.erase(typeid(C));
  std::cout << "Removed component " << typeid(C).name() << std::endl;
}

template <typename C> bool Entity::checkComponent() {
  auto it = components.find(typeid(C));
  return it != components.end();
}

#endif // ENTITY_H
