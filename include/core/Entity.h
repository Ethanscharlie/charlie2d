#ifndef ENTITY_H
#define ENTITY_H

#include "GameManager.h"
#include <SDL.h>
#include <functional>
#include <map>
#include <stdexcept>
#include <typeindex>
#include <vector>

class Animation;
class Component;
class entityBox;
//

class Entity;

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
  template <typename C> C *add() {
    if (checkComponent<C>()) {
      return get<C>();
    }

    C *component = new C();

    component->entity = this;
    component->entityTag = tag;
    component->start();
    components[typeid(C)] = component;

    component->index = iid; // components[typeid(C)].size();
    GameManager::addComponent<C>(component);

    return component;
  }

  /**
   * \brief Either adds or gets a Component depending on its existance (won't
   * override components)
   */
  template <typename C> C *require() {
    if (checkComponent<C>()) {
      return get<C>();
    } else {
      return add<C>();
    }
  }

  /**
   * \brief Gets a component from the entity Ex: get<Sprite>()
   */
  template <typename C> C *get() {
    auto it = components.find(typeid(C));
    if (it != components.end()) {
      if (it->second == nullptr)
        throw std::runtime_error("Component was nullptr");
      return static_cast<C *>(it->second);

    } else
      throw std::runtime_error("Component not found!");
  }

  /**
   * \brief Removes and deletes a component
   */
  template <typename C> void remove() {
    // Remove from GameManager
    C *component = get<C>();
    GameManager::removeComponent(component, typeid(C));

    // Remove from entity
    components.erase(typeid(C));

    delete component;

    std::cout << "Removed component " << typeid(C).name() << std::endl;
  }

  /**
   * \brief Does the enity have this? Ex: checkComponent<Sprite>()
   */
  template <typename C> bool checkComponent() {
    auto it = components.find(typeid(C));
    return it != components.end();
  }

  void update() {
    for (auto [type, component] : components) {
      if (!component->standardUpdate)
        continue;
      if (!GameManager::updateEntities && !component->typeIsRendering)
        continue;
      component->update(GameManager::deltaTime);
    }
  }

  /**
   * Gets the entitys components
   */
  std::map<std::type_index, Component *> gets() { return components; };

  /**
   * \brief Adds a child to the entity
   */
  void addChild(Entity *entity) { children.push_back(entity); }

  /**
   * \brief Sets the parent of the entity
   */
  void setParent(Entity *entity) {
    parent = entity;
    entity->addChild(this);
  }

  /**
   * \brief Gets the parent of the entity
   */
  Entity *getParent() { return parent; }
  std::vector<Entity *> getChildren() { return children; }
  void removeParent(){};

  /**
   * \brief If marked true the entity and all its components will be removed and
   * deleted from memory
   */
  bool toDestroy = false;
  /**
   * \brief String tag for the entity
   */
  std::string tag = "";
  bool debug = false;
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
  bool useLayer = false;

  /**
   *\brief A pointer to the entitys entityBox
   */
  entityBox *box = nullptr;

  std::map<std::type_index, Component *> components;

  std::vector<Entity *> children;

  Entity *parent = nullptr;


private:
};


#endif // ENTITY_H
