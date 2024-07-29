#pragma once
#include "Camera.hpp"
#include "Component.hpp"
#include "Entity.hpp"
#include "GameManager.hpp"
#include "InputManager.hpp"
#include "Math.hpp"
#include "Serializer.hpp"
#include "Vector2f.hpp"
#include <SDL.h>

/**
 *\brief Meant to be used alongside UISliceRenderer
 */
class Button : public Component {
public:
  Button() {
    propertyRegister = {
        GET_PROP(onClick),  GET_PROP(onHold),       GET_PROP(onHover),
        GET_PROP(offHover), GET_PROP(checkInWorld),
    };
  };

  void start() override;

  void update(float deltaTime) override;

  std::function<void()> onClick = []() {};
  std::function<void()> onHold = []() {};
  std::function<void()> onHover = []() {};
  std::function<void()> offHover = []() {};

  bool checkInWorld = false;
  bool touching = false;
};
REGISTER_COMPONENT_TYPE(Button);
