#include "ControllerManager.h"
#include "SDL_gamecontroller.h"
#include <iostream>

std::vector<Controller> ControllerManager::controllers;

ControllerManager::ControllerManager() { resetTriggerButtons(); }

void ControllerManager::onControllerConnect(int joystickID) {
  std::cout << "Controller connected " << joystickID << "\n";
  SDL_GameController *controller = SDL_GameControllerOpen(joystickID);

  // Look for disconnected controllers
  int index = 0;
  for (Controller &listedController : controllers) {
    if (listedController.controller == nullptr) {
      controllers[index] = Controller(controller);
      return;
    }
    index++;
  }

  // Add if new player
  controllers.push_back(controller);
}

void ControllerManager::onControllerDisconnect(int joystickID) {
  int index = 0;
  for (Controller &listedController : controllers) {
    int listedJoystickID = SDL_JoystickInstanceID(
        SDL_GameControllerGetJoystick(listedController.controller));
    if (joystickID == listedJoystickID) {
      controllers[index].controller = nullptr;
      SDL_GameControllerClose(listedController.controller);
      return;
    }
    index++;
  }
}

void ControllerManager::resetTriggerButtons() {
  for (Controller &controller : controllers) {
    controller.resetTriggerButtons();
  }
}

Controller *ControllerManager::findController(int joystickID) {
  for (Controller &controller : controllers) {
    int controllerJoystickID = SDL_JoystickInstanceID(
        SDL_GameControllerGetJoystick(controller.controller));
    if (controllerJoystickID == joystickID) {
      return &controller;
    }
  }

  return nullptr;
}
