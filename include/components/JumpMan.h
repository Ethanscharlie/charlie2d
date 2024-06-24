#pragma once
#include "Entity.h"
#include "GameManager.h"
#include "Serializer.h"
#include "physicsBody.h"

/**
 * \brief This class is made to be a simple Platformer player controller that
 * uses the physicsBody
 */
class JumpMan : public Component {
public:
  JumpMan() {
    propertyRegister = {
        GET_PROP(gravity),    GET_PROP(speed),          GET_PROP(airSpeed),
        GET_PROP(maxSpeed),   GET_PROP(tracktion),      GET_PROP(jumpPeak),
        GET_PROP(jumpChange), GET_PROP(jumps),          GET_PROP(allowJump),
        GET_PROP(needGround), GET_PROP(touchingGround),
    };
  }

  void start() override { entity->require<physicsBody>(); }

  void update(float deltaTime) override {
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
    for (Tilemap *col : GameManager::getComponents<Tilemap>()) {
      if (col->tileGrid.checkCollision(groundCheckBox)) {
        touchingGround = true;
      }
    }

    if (InputManager::checkInput("jumpTrigger")) {
      std::cout << "FPS: " << 1.0f / deltaTime << std::endl;
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
        for (Tilemap *col : GameManager::getComponents<Tilemap>()) {
          if (col->tileGrid.checkCollision(groundCheckBox)) {
            checkground = true;
            break;
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
    }

    entity->get<physicsBody>()->velocity.y += gravity * deltaTime;

    if (InputManager::checkInput("jump")) {
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

  /**
   * \brief Fall go brrr (is multipyed by deltaTime so it should be a large
   * number)
   */
  float gravity = 1500;
  /**
   * \brief Horizontal move speed (Adds up like gravity until maxSpeed)
   */
  float speed = 40;
  /**
   * \brief Horizontal move speed but in air
   */
  float airSpeed = 40;
  /**
   * \brief Max the speed can increase too
   */
  float maxSpeed = 200;
  /**
   * \brief How fast Horizontal movment slows down
   */
  float tracktion = 1000;
  /**
   * \brief How high jumpChange and get (Soon to be replaced with a time value)
   */
  float jumpPeak = 80;
  /**
   * \brief How much your jump changes while holding down the space key
   */
  float jumpChange = 300;
  /**
   * \brief For double jumping and stuffs
   */
  int jumps = 1;

  /**
   * \brief No more jumping
   */
  bool allowJump = true;
  /**
   * \brief Who even cares about physics mid are jumps are the future
   */
  bool needGround = true;
  /**
   * \brief Uhh this should probably be a private value...
   */
  bool touchingGround = true;

  float jumpsCounter;

private:
  float jumpAmount = 0;
  bool jumping = false;
  Box groundCheckBox = {0, 0, 0, 0};
};
REGISTER_COMPONENT_TYPE(JumpMan);
