#ifndef ENTITY_H
#define ENTITY_H
#include "Math.h"
#include "Scene.h"
#include <SDL.h>
#include <cmath>
#include <map>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

class Scene;
class Animation;
class Component;

  /**
   * \brief Entitys are containers for an EnityBox and components
   */
class Entity {
public:
  Entity(){
      // box = new entityBox(this);
  };

  ~Entity() {  }

  /**
   * \brief OLD DON'T USE THIS
   */
  template <typename C> void addComponent() {
    C *component = new C();

    component->entity = this;
    component->box = box;
    component->entityTag = tag;
    component->scene = scene;
    component->start();
    components[typeid(C)] = component;

    // scene->template add<C>(components[typeid(C)]);
    if (scene->components.count(typeid(C)) ==
        0) { //&& std::find(componentTypes.begin(), componentTypes.end(),
             //typeid(C)) == componentTypes.end()) {
      std::cout << "added type " << component->title << std::endl;
    }

    component->index = 0; // components[typeid(C)].size();
    scene->components[typeid(C)].push_back(component);
  }

  /**
   * \brief OLD DON'T USE THIS
   */
  template <typename C> C &getComponent() {
    auto it = components.find(typeid(C));
    if (it != components.end())
      return *static_cast<C *>(it->second);
    else
      throw std::runtime_error("Component not found!");
  }

  /**
   * \brief Adds a component to the entity Ex: add<Sprite>()
   */
  template <typename C> C *add() {
    C *component = new C();

    component->entity = this;
    component->box = box;
    component->entityTag = tag;
    component->scene = scene;
    component->start();
    components[typeid(C)] = component;

    // scene->template add<C>(components[typeid(C)]);
    if (scene->components.count(typeid(C)) ==
        0) { //&& std::find(componentTypes.begin(), componentTypes.end(),
             //typeid(C)) == componentTypes.end()) {
      std::cout << "added type " << component->title << std::endl;
    }

    component->index = 0; // components[typeid(C)].size();
    scene->components[typeid(C)].push_back(component);

    return component;
  }

  /**
   * \brief Either adds or gets a Component depending on its existance (won't override components)
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
    if (it != components.end())
      return static_cast<C *>(it->second);
    else
      throw std::runtime_error("Component not found!");
  }

  /**
   * \brief Does the enity have this? Ex: checkComponent<Sprite>()
   */
  template <typename C> bool checkComponent() {
    auto it = components.find(typeid(C));
    return it != components.end();
  }

  std::map<std::type_index, Component *> getComponents() { return components; };

  /**
   * \brief 
   */
  void addChild(Entity *entity) { children.push_back(entity); }

  /**
   * \brief 
   */
  void setParent(Entity *entity) {
    parent = entity;
    entity->addChild(this);
  }

  /**
   * \brief 
   */
  Entity *getParent() { return parent; }
  std::vector<Entity *> getChildren() { return children; }
  void removeParent(){};

  /**
   * \brief If marked true the entity and all its components will be removed and deleted from memory 
   */
  bool toDestroy = false;
  /**
   * \brief The Current Scene 
   */
  Scene *scene = nullptr;
  entityBox *box;
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

  std::map<std::type_index, Component *> components;

private:
  std::vector<Entity *> children;
  Entity *parent = nullptr;
};

#endif // ENTITY_H
