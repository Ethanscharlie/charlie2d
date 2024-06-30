#ifndef ENTITY_H
#define ENTITY_H

#include "Box.h"
#include "GameManager.h"
#include <SDL.h>
#include <functional>
#include <map>
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
   * \brief Adds a component to the entity Ex: add<Sprite>()
   */
  template <typename C> C *add(bool start = true);
  /**
   * \brief Gets a component from the entity Ex: get<Sprite>()
   */
  template <typename C> C *get();

  /**
   * \brief Adds a component to the entity
   */
  template <typename C> C &addComponent(bool start = true);
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

  void remove(std::type_index type);

  void update();

  void changeTag(std::string newTag);
  /**
   * Gets the entitys components
   */
  std::map<std::type_index, Component *> gets() { return components; };

  /**
   * \brief If marked true the entity and all its components will be removed and
   * deleted from memory
   */
  bool toDestroy = false;
  /**
   * \brief String tag for the entity
   */
  std::string tag = "";

  std::string name = "";

  std::string group = "";

  bool debug = false;

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
   * \brief Tells the engine to update with a sorting algorithim (made for
   * sprites)
   */
  bool useLayer = true;

  /**
   *\brief A pointer to the entitys entityBox
   */
  Box box;

  std::map<std::type_index, Component *> components;

private:
};

//
//
//
//
//
//
//

template <typename C> C *Entity::add(bool start) {
  if (checkComponent<C>()) {
    return get<C>();
  }

  C *component = new C();

  component->entity = this;
  component->entityTag = tag;
  if (start)
    component->start();
  components[typeid(C)] = component;

  component->index = iid; // components[typeid(C)].size();
  GameManager::addComponent<C>(component);

  return component;
}

template <typename C> C *Entity::get() {
  auto it = components.find(typeid(C));
  if (it != components.end()) {
    if (it->second == nullptr)
      throw std::runtime_error("Component was nullptr");
    return static_cast<C *>(it->second);

  } else
    throw std::runtime_error("Component not found!");
}

template <typename C> C &Entity::addComponent(bool start) {
  if (checkComponent<C>()) {
    return get<C>();
  }

  C *component = new C();

  component->entity = this;
  component->entityTag = tag;
  if (start)
    component->start();
  components[typeid(C)] = component;

  component->index = iid; // components[typeid(C)].size();
  GameManager::addComponent<C>(component);

  return component;
}

template <typename C> C &Entity::getComponent() {
  auto it = components.find(typeid(C));
  if (it != components.end()) {
    if (it->second == nullptr)
      throw std::runtime_error("Component was nullptr");
    return static_cast<C *>(it->second);

  } else
    throw std::runtime_error("Component not found!");
}

template <typename C> void Entity::remove() {
  // Remove from GameManager
  C *component = get<C>();
  GameManager::removeComponent(component, typeid(C));

  // Remove from entity
  components.erase(typeid(C));

  delete component;

  std::cout << "Removed component " << typeid(C).name() << std::endl;
}

template <typename C> bool Entity::checkComponent() {
  auto it = components.find(typeid(C));
  return it != components.end();
}

#endif // ENTITY_H
