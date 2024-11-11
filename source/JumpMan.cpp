#include "JumpMan.hpp"
#include "Component.hpp"
#include "Event.hpp"
#include "GameManager.hpp"
#include "SDL_keycode.h"
#include "SDL_scancode.h"
#include "ldtk/LDTK_Tilemap.hpp"
#include "physicsBody.hpp"

JumpMan::JumpMan(Entity& entity) : Component(entity) {
  Event::addEventListener("SpaceKeyDown", [this]() {
    std::cout << "FPS: " << 1.0f / GameManager::getDeltaTime() << std::endl;
    if (!allowJump)
      return;

    bool checkground = false;
    if (needGround) {
      groundCheckBox = this->entity.box;
      groundCheckBox.position.y += this->entity.box.size.y;
      for (SolidBody *col : GameManager::getComponents<SolidBody>()) {
        if (col->entity.box.checkCollision(groundCheckBox)) {
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
        this->entity.getComponent<physicsBody>().velocity.y = 0;
      jumping = true;
      // gravity = 1500;
    }
  });
}

void JumpMan::update() {
  entity.addComponent<physicsBody>();

  if (GameManager::getDeltaTime() > 0.2)
    return;
  if (abs(entity.getComponent<physicsBody>().velocity.x) < maxSpeed) {
    if (touchingGround) {
      entity.getComponent<physicsBody>().velocity.x +=
          InputManager::checkHorizontal() * speed;
    } else {
      entity.getComponent<physicsBody>().velocity.x +=
          InputManager::checkHorizontal() * airSpeed;
    }
  }

  if (abs(entity.getComponent<physicsBody>().velocity.x) < 1) {
    entity.getComponent<physicsBody>().velocity.x = 0;
  } else {
    if (entity.getComponent<physicsBody>().velocity.x > 0) {
      entity.getComponent<physicsBody>().velocity.x -= tracktion * GameManager::getDeltaTime();
    } else {
      entity.getComponent<physicsBody>().velocity.x += tracktion * GameManager::getDeltaTime();
    }
  }

  touchingGround = false;
  groundCheckBox = entity.box;
  groundCheckBox = entity.box;
  groundCheckBox.position.y += entity.box.size.y;
  for (SolidBody *col : GameManager::getComponents<SolidBody>()) {
    if (col->entity.box.checkCollision(groundCheckBox)) {
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

  entity.getComponent<physicsBody>().velocity.y += gravity * GameManager::getDeltaTime();

  if (InputManager::checkKeyHeld(SDL_SCANCODE_SPACE)) {
    // Keep Jump
    if (jumping) {
      if (jumpAmount <= jumpPeak) {
        entity.getComponent<physicsBody>().velocity.y = -jumpChange;
        jumpAmount += jumpChange * GameManager::getDeltaTime();
      }
    }
  } else {
    jumpAmount = 0;
    jumping = false;
  }
}
