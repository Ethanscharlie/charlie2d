#pragma once
#include "Camera.h"
#include "Component.h"
#include "Entity.h"
#include "EntityBox.h"
#include "GameManager.h"
#include "InputManager.h"
#include "Math.h"
#include "Serializer.h"
#include "Vector2f.h"
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

  void start() override {}

  void update(float deltaTime) override {
    bool touching = false;
    Vector2f mousePos;
    if (checkInWorld) {
      mousePos = InputManager::getMouseWorldPosition();
    } else {
      mousePos = InputManager::getMouseUIPosition();
    }

    touching = entity->box.getLeft() < mousePos.x &&
               entity->box.getRight() > mousePos.x &&
               entity->box.getTop() < mousePos.y &&
               entity->box.getBottom() > mousePos.y;

    if (touching) {
      onHover();
      if (InputManager::checkInput("fire")) {
        onClick();
      }
      if (InputManager::mouseHeld) {
        onHold();
      }
    } else {
      offHover();
    }
  }

  std::function<void()> onClick = []() {};
  std::function<void()> onHold = []() {};
  std::function<void()> onHover = []() {};
  std::function<void()> offHover = []() {};

  bool checkInWorld = false;
};
REGISTER_COMPONENT_TYPE(Button);
