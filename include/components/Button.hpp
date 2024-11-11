#pragma once
#include "Camera.hpp"
#include "Component.hpp"
#include "Entity.hpp"
#include "GameManager.hpp"
#include "InputManager.hpp"
#include "Math.hpp"
#include "Vector2f.hpp"
#include <SDL.h>

/**
 *\brief Meant to be used alongside UISliceRenderer
 */
class Button : public Component {
public:
  Button(Entity &entity);
  void update() override;

  void onDestroy() override;

  std::function<void()> onClick = []() {};
  std::function<void()> onHold = []() {};
  std::function<void()> onHover = []() {};
  std::function<void()> offHover = []() {};

  int eventid = 0;
  bool checkInWorld = false;
  bool touching = false;
};
