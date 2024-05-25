#pragma once

#define NUM_BUTTONS 15

#include "SDL_gamecontroller.h"
#include <vector>

struct Controller {
  Controller(SDL_GameController *controller) { this->controller = controller; }
  SDL_GameController *controller;
  bool triggerButtons[NUM_BUTTONS];

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
  static void resetTriggerButtons();
  static Controller* findController(int joystickID);

  // Will be nullptr if disconnected
  static std::vector<Controller> controllers;
};
