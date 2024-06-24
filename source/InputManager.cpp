#include "InputManager.h"
#include "Camera.h"
#include "GameManager.h"
#include "Vector2f.h"

bool InputManager::mousePressed = false;
bool InputManager::jumpPressed = false;
bool InputManager::mouseHeld = false;
bool InputManager::rightClick = false;

bool InputManager::leftMouseHeld = false;
bool InputManager::rightMouseHeld = false;

float InputManager::mouseScroll = 0;

bool InputManager::keys[NUM_KEYS];
bool InputManager::keysUped[NUM_KEYS];

InputManager::InputManager() {
  for (int i = 0; i < NUM_KEYS; ++i) {
    keys[i] = false; // Initialize all keys as released
    keysUped[i] = true;
  }
}

void InputManager::update() {
  mousePressed = false;
  jumpPressed = false;
  rightClick = false;
  InputManager::mouseScroll = 0;

  for (int i = 0; i < NUM_KEYS; ++i) {
    keys[i] = false; // Initialize all keys as released
  }
}

bool InputManager::checkInput(const std::string &input) {
  SDL_PumpEvents();
  const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
  //    std::cout << keyboardState << std::endl;

  if (input == "any") {
    for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
      if (keyboardState[i]) {
        return true;
      }
    }

    return false;
  }
  if (input == "fire") {
    if (mousePressed) {
      return true;
    }
  } else if (input == "jumpTrigger") {
    if (jumpPressed) {
      return true;
    }
  } else if (input == "jump") {
    if (keyboardState[SDL_SCANCODE_SPACE]) {
      return true;
    }
  } else if (input == "dash") {
    if (keyboardState[SDL_SCANCODE_LSHIFT] ||
        keyboardState[SDL_SCANCODE_LSHIFT]) {
      return true;
    }
  } else if (input == "rightBump") {
    if (keyboardState[SDL_SCANCODE_E]) {
      return true;
    }
  } else if (input == "leftBump") {
    if (keyboardState[SDL_SCANCODE_Q]) {
      return true;
    }
  } else if (input == "hit") {
    if (keyboardState[SDL_SCANCODE_F]) {
      return true;
    }
  } else if (input == "take") {
    if (keyboardState[SDL_SCANCODE_E]) {
      return true;
    }
  } else if (input == "up") {
    if (keyboardState[SDL_SCANCODE_W] or keyboardState[SDL_SCANCODE_UP]) {
      return true;
    }
  } else if (input == "down") {
    if (keyboardState[SDL_SCANCODE_S] or keyboardState[SDL_SCANCODE_DOWN]) {
      return true;
    }
  } else if (input == "right") {
    if (keyboardState[SDL_SCANCODE_D] or keyboardState[SDL_SCANCODE_RIGHT]) {
      return true;
    }
  } else if (input == "left") {
    if (keyboardState[SDL_SCANCODE_A] or keyboardState[SDL_SCANCODE_LEFT]) {
      return true;
    }
  }

  return false;
}

int InputManager::checkVertical() {
  if (checkInput("up"))
    return 1;
  if (checkInput("down"))
    return -1;
  return 0;
}

int InputManager::checkHorizontal() {
  if (checkInput("right"))
    return 1;
  if (checkInput("left"))
    return -1;
  return 0;
}

Vector2f InputManager::checkAxis() {
  return {static_cast<float>(checkHorizontal()),
          static_cast<float>(-checkVertical())};
}

Vector2f InputManager::getMouseWorldPosition() {
  Vector2f mouse = getMouseScreenPosition();

  // float scaler = GameManager::screen_change_scale *
  // ((GameManager::gameWindowSize.x +
  //             GameManager::gameWindowSize.y) / (GameManager::camera.size.x +
  //             GameManager::camera.size.y));
  // return (mouse - GameManager::currentWindowSize / 2) / scaler +
  // GameManager::camera.getCenter();
  return (mouse - GameManager::gameWindowSize / 2) / Camera::getScale() +
         Camera::getPosition();
}

Vector2f InputManager::getMouseUIPosition() {
  Vector2f mouse = getMouseScreenPosition();
  return (mouse - GameManager::gameWindowSize / 2);
}

Vector2f InputManager::getMouseScreenPosition() {
  int virtualWidth, virtualHeight;
  SDL_RenderGetLogicalSize(GameManager::renderer, &virtualWidth,
                           &virtualHeight);

  int windowWidth, windowHeight;
  SDL_GetWindowSize(GameManager::window, &windowWidth, &windowHeight);

  int mouseX, mouseY;
  SDL_GetMouseState(&mouseX, &mouseY);

  double scale;
  int xOffset, yOffset;
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

void InputManager::onEvent(SDL_Event event) {
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    InputManager::mousePressed = true;
    InputManager::mouseHeld = true;

    if (event.button.button == SDL_BUTTON_RIGHT) {
      InputManager::rightClick = true;
      InputManager::rightMouseHeld = true;
    }
    if (event.button.button == SDL_BUTTON_LEFT) {
      InputManager::leftMouseHeld = true;
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
      InputManager::keys[event.key.keysym.sym] = true;
    }

    switch (event.key.keysym.sym) {
    case SDLK_SPACE:
      InputManager::jumpPressed = true;
    }
  }
}
