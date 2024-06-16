#pragma once

#include "Vector2f.h"
#define NUM_BUTTONS 15
#define BIT16_MAX 32767.0

#include "SDL_gamecontroller.h"
#include <vector>

struct Controller {
  Controller(SDL_GameController *controller) { this->controller = controller; }
  SDL_GameController *controller;
  bool triggerButtons[NUM_BUTTONS];
  Vector2f leftAxis;
  Vector2f rightAxis;
  float leftTrigger = 0;
  float rightTrigger = 0;

  void resetTriggerButtons() {
    for (int i = 0; i < NUM_BUTTONS; ++i) {
      triggerButtons[i] = false; // Initialize all keys as released
    }
  }
};

class ControllerManager {
public:
  ControllerManager();
  static void onControllerConnect(int joystickID);
  static void onControllerDisconnect(int joystickID);
  static void onStickEvent(int joystickID, Uint8 sdl2Axis,
                           Sint16 axisValue);
  static void resetTriggerButtons();
  static Controller *findController(int joystickID);

  // Will be nullptr if disconnected
  static std::vector<Controller> controllers;
};
