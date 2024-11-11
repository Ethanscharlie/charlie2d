#include "InputManager.hpp"

#include "Camera.hpp"
#include "Event.hpp"
#include "GameManager.hpp"
#include "SDL_scancode.h"
#include "Vector2f.hpp"
#include <math.h>

bool InputManager::mouseHeld = false;

bool InputManager::leftMouseHeld = false;
bool InputManager::rightMouseHeld = false;

float InputManager::mouseScroll = 0;

bool InputManager::keyEvents[NUM_KEYS];

InputManager::InputManager() {
  for (bool &keyEvent : keyEvents) {
    keyEvent = false; // Initialize all keys as released
  }
}

void InputManager::update() {
  InputManager::mouseScroll = 0;

  for (bool &keyEvent : keyEvents) {
    keyEvent = false; // Initialize all keys as released
  }
}

auto InputManager::checkVertical() -> int {
  if (checkKeyHeld(SDL_SCANCODE_W) || checkKeyHeld(SDL_SCANCODE_UP))
    return 1;
  if (checkKeyHeld(SDL_SCANCODE_S) || checkKeyHeld(SDL_SCANCODE_DOWN))
    return -1;
  return 0;
}

auto InputManager::checkHorizontal() -> int {
  if (checkKeyHeld(SDL_SCANCODE_D) || checkKeyHeld(SDL_SCANCODE_RIGHT))
    return 1;
  if (checkKeyHeld(SDL_SCANCODE_A) || checkKeyHeld(SDL_SCANCODE_LEFT))
    return -1;
  return 0;
}

auto InputManager::checkAxis() -> Vector2f {
  return {static_cast<float>(checkHorizontal()),
          static_cast<float>(-checkVertical())};
}

auto InputManager::getMouseWorldPosition() -> Vector2f {
  Vector2f mouse = getMouseScreenPosition();

  // float scaler = GameManager::screen_change_scale *
  // ((GameManager::getGameWindowSize().x +
  //             GameManager::getGameWindowSize().y) / (GameManager::camera.size.x +
  //             GameManager::camera.size.y));
  // return (mouse - GameManager::currentWindowSize / 2) / scaler +
  // GameManager::camera.getCenter();
  return (mouse - GameManager::getGameWindowSize() / 2) / Camera::getScale() +
         Camera::getPosition();
}

auto InputManager::getMouseUIPosition() -> Vector2f {
  Vector2f mouse = getMouseScreenPosition();
  return (mouse - GameManager::getGameWindowSize() / 2);
}

auto InputManager::getMouseScreenPosition() -> Vector2f {
  int virtualWidth = 0, virtualHeight = 0;
  SDL_RenderGetLogicalSize(GameManager::getRenderer(), &virtualWidth,
                           &virtualHeight);

  int windowWidth = 0, windowHeight = 0;
  SDL_GetWindowSize(GameManager::getWindow(), &windowWidth, &windowHeight);

  int mouseX = 0, mouseY = 0;
  SDL_GetMouseState(&mouseX, &mouseY);

  double scale = NAN;
  int xOffset = 0, yOffset = 0;
  if (windowWidth * virtualHeight > windowHeight * virtualWidth) {
    // Calculate the scale based on height
    scale = (double)windowHeight / virtualHeight;
    xOffset = (windowWidth - virtualWidth * scale) / 2;
    yOffset = 0;
  } else {
    // Calculate the scale based on width
    scale = (double)windowWidth / virtualWidth;
    xOffset = 0;
    yOffset = (windowHeight - virtualHeight * scale) / 2;
  }

  float logicalX = 1.0f * (mouseX - xOffset) / scale;
  float logicalY = 1.0f * (mouseY - yOffset) / scale;

  return {logicalX, logicalY};
}

auto InputManager::checkKeyHeld(SDL_Scancode scancode) -> bool {
  SDL_PumpEvents();
  const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
  return keyboardState[scancode];
}

void InputManager::onEvent(SDL_Event event) {
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    Event::fireEvent("MouseButtonDown");
    InputManager::mouseHeld = true;

    if (event.button.button == SDL_BUTTON_RIGHT) {
      Event::fireEvent("RightMouseButtonDown");
      InputManager::rightMouseHeld = true;
    }
    if (event.button.button == SDL_BUTTON_LEFT) {
      Event::fireEvent("LeftMouseButtonDown");
      InputManager::leftMouseHeld = true;
    }
  }

  if (event.type == SDL_MOUSEBUTTONUP) {
    Event::fireEvent("MouseButtonUp");

    if (event.button.button == SDL_BUTTON_RIGHT) {
      Event::fireEvent("RightMouseButtonUp");
    }
    if (event.button.button == SDL_BUTTON_LEFT) {
      Event::fireEvent("LeftMouseButtonUp");
    }
  }

  else if (event.type == SDL_MOUSEBUTTONUP) {
    InputManager::mouseHeld = false;

    if (event.button.button == SDL_BUTTON_RIGHT) {
      InputManager::rightMouseHeld = false;
    }
    if (event.button.button == SDL_BUTTON_LEFT) {
      InputManager::leftMouseHeld = false;
    }
  }

  else if (event.type == SDL_MOUSEWHEEL) {
    InputManager::mouseScroll = event.wheel.y;
  }

  else if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym <= 256) {
      InputManager::keyEvents[event.key.keysym.sym] = true;
    }

    switch (event.key.keysym.sym) {
    case SDLK_SPACE:
      Event::fireEvent("SpaceKeyDown");
    }
  }
}
