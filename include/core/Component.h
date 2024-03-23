#ifndef COMPONENT_H
#define COMPONENT_H

#include <iostream>
#include <string>
#include <typeindex>
#include <utility>
#include <vector>

struct entityBox;
class Entity;

// struct EditorDataItem {
//   EditorDataItem(std::string _name, void *_value, std::type_index _type,
//                  std::type_index _componentType)
//       : name(_name), value(_value), type(_type),
//       componentType(_componentType) {
//   }
//
//   std::string name;
//   void *value;
//   std::type_index type;
//   std::type_index componentType;
// };
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
  Component(std::string _title = "") : title(_title){};
  virtual ~Component() {}
  /**
   * \brief Runs after Component data is set in entity.add
   */
  virtual void start() {}
  /**
   * \brief Updates in the update loop
   */
  virtual void update(float deltaTime) {}
  /**
   * \brief Is called when entity->toDestory = true
   */
  virtual void onDestroy() {}
  /**
   * \brief Runs when the screen size is changed
   */
  virtual void onScreenChange() {}

  /**
   * \brief The attached entity
   */
  Entity *entity = nullptr;
  std::string title = "";
  bool isInit = false;
  int index;
  bool typeIsRendering = false;
  std::string entityTag = "";

  std::vector<PropertyData> propertyRegister = {};

  /**
   * \brief Will update the Component in the game loop
   */
  bool standardUpdate = true;
};

#endif
