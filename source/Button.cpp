#include "Button.h"

void Button::update(float deltaTime) {
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
