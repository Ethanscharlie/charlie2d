#ifndef COMPONENT_H
#define COMPONENT_H

#include <iostream>
#include <string>
#include <typeindex>
#include <utility>
#include <vector>

class Entity;

struct PropertyData {
  template <typename T>
  PropertyData(std::string _name, T *_value)
      : name(_name), value(_value), type(typeid(T)) {}
  std::string name;
  void *value;
  std::type_index type;
};

/**
 * \brief Components are made to attach to Entity s Sprite Components with
 * handle rendering while Collider s will handle collisions
 */
class Component {
public:
  Component(Entity &entity) : entity(entity){};
  /**
   * \brief Updates in the update loop
   */
  virtual void update() {}
  /**
   * \brief Is called when entity->toDestory = true
   */
  virtual void onDestroy() {}

  /**
   * \brief The attached entity
   */
  Entity &entity;
  std::string title = "";
  bool isInit = false;
  bool typeIsRendering = false;

  std::vector<PropertyData> propertyRegister = {};

  /**
   * \brief Will update the Component in the game loop
   */
  bool standardUpdate = true;
};

#endif
