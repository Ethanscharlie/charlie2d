#pragma once
#include "Entity.hpp"
#include "GameManager.hpp"

/**
 * \brief This class is made to be a simple Platformer player controller that
 * uses the physicsBody
 */
class JumpMan : public Component {
public:
  JumpMan(Entity &entity);
  void update() override;

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
