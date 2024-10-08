#include "JumpMan.hpp"
#include "Event.hpp"
#include "SDL_keycode.h"
#include "SDL_scancode.h"
#include "ldtk/LDTK_Tilemap.hpp"

void JumpMan::start() {
  Event::addEventListener("SpaceKeyDown", [this]() {
    std::cout << "FPS: " << 1.0f / GameManager::deltaTime << std::endl;
    if (!allowJump)
      return;

    bool checkground = false;
    if (needGround) {
      groundCheckBox = entity->box;
      groundCheckBox.position.y += entity->box.size.y;
      for (SolidBody *col : GameManager::getComponents<SolidBody>()) {
        if (col->entity->box.checkCollision(groundCheckBox)) {
          checkground = true;
          break;
        }
      }
      for (LDTK::Tilemap *col : GameManager::getComponents<LDTK::Tilemap>()) {
        if (!col->layer)
          continue;
        if (!col->solid)
          continue;
        for (Box &box : col->layer->boxes) {
          if (box.checkCollision(groundCheckBox)) {
            checkground = true;
            break;
          }
        }
      }
    } else
      checkground = true;

    if (checkground || jumpsCounter > 0) {
      if (checkground)
        jumpsCounter = jumps;
      jumpsCounter--;
      // Start Jump
      if (!jumping)
        entity->get<physicsBody>()->velocity.y = 0;
      jumping = true;
      // gravity = 1500;
    }
  });
}

void JumpMan::update(float deltaTime) {
  entity->add<physicsBody>();

  if (deltaTime > 0.2)
    return;
  if (abs(entity->get<physicsBody>()->velocity.x) < maxSpeed) {
    if (touchingGround) {
      entity->get<physicsBody>()->velocity.x +=
          InputManager::checkHorizontal() * speed;
    } else {
      entity->get<physicsBody>()->velocity.x +=
          InputManager::checkHorizontal() * airSpeed;
    }
  }

  if (abs(entity->get<physicsBody>()->velocity.x) < 1) {
    entity->get<physicsBody>()->velocity.x = 0;
  } else {
    if (entity->get<physicsBody>()->velocity.x > 0) {
      entity->get<physicsBody>()->velocity.x -= tracktion * deltaTime;
    } else {
      entity->get<physicsBody>()->velocity.x += tracktion * deltaTime;
    }
  }

  touchingGround = false;
  groundCheckBox = entity->box;
  groundCheckBox = entity->box;
  groundCheckBox.position.y += entity->box.size.y;
  for (SolidBody *col : GameManager::getComponents<SolidBody>()) {
    if (col->entity->box.checkCollision(groundCheckBox)) {
      touchingGround = true;
    }
  }

  for (LDTK::Tilemap *col : GameManager::getComponents<LDTK::Tilemap>()) {
    if (!col->layer)
      continue;
    if (!col->solid)
      continue;
    for (Box &box : col->layer->boxes) {
      if (box.checkCollision(groundCheckBox)) {
        touchingGround = true;
      }
    }
  }

  entity->get<physicsBody>()->velocity.y += gravity * deltaTime;

  if (InputManager::checkKeyHeld(SDL_SCANCODE_SPACE)) {
    // Keep Jump
    if (jumping) {
      if (jumpAmount <= jumpPeak) {
        entity->get<physicsBody>()->velocity.y = -jumpChange;
        jumpAmount += jumpChange * deltaTime;
      }
    }
  } else {
    jumpAmount = 0;
    jumping = false;
  }
}
