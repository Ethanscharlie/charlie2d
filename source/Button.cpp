#include "Button.hpp"
#include "Event.hpp"

Button::Button(Entity &entity) : Component(entity) {
  eventid = Event::addEventListener("LeftMouseButtonDown", [this]() {
    if (touching)
      onClick();
  });
}

void Button::update() {
  touching = false;
  Vector2f mousePos;
  if (checkInWorld) {
    mousePos = InputManager::getMouseWorldPosition();
  } else {
    mousePos = InputManager::getMouseUIPosition();
  }

  touching =
      entity.box.getLeft() < mousePos.x && entity.box.getRight() > mousePos.x &&
      entity.box.getTop() < mousePos.y && entity.box.getBottom() > mousePos.y;

  if (touching) {
    onHover();
    if (InputManager::mouseHeld) {
      onHold();
    }
  } else {
    offHover();
  }
}

void Button::onDestroy() { Event::removeEventListener(eventid); }
