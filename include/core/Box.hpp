#pragma once
#include "SDL_rect.h"
#include "Vector2f.hpp"
#include <vector>

enum class BoxCollisionCheck { All, VerticaleOnly, HorizontalOnly };

class Entity;
struct Box;
struct slideOut {
  std::vector<Box *> hitList;
  bool horizontalHit = false;
  bool verticleHit = false;
};

/**
 * \brief Two Vector2f s put together
 *
 * Boxes are simple datatypes composed of two vectors, a position (top left
 * corner) and size
 */
struct Box {
  Box() {}

  Box(float p_x, float p_y, float p_w, float p_h)
      : position(p_x, p_y), size(p_w, p_h) {}

  /**
   * \brief Gets the X position for the right side
   */
  float getRight();
  /**
   * \brief Gets the X position for the left side
   */
  float getLeft();
  /**
   * \brief Gets the Y position for the Top side
   */
  float getTop();
  /**
   * \brief Gets the Y position for the Bottom side
   */
  float getBottom();

  Vector2f getTopLeftCorner();
  Vector2f getTopMiddle();
  Vector2f getTopRightCorner();
  Vector2f getBottomLeftCorner();
  Vector2f getBottomMiddle();
  Vector2f getMiddleLeft();
  Vector2f getMiddleRight();
  Vector2f getBottomRightCorner();
  Vector2f getCenter();
  /**
   * \brief Sets the size but also moves the object to still be centered in the
   * same position
   */
  void setScale(const Vector2f &point);
  /**
   * \brief Changes the size (Adds) but also moves the object to still be
   * centered in the same position
   */
  void changeScale(const Vector2f &change);
  /**
   * \brief Sets the center of the box to the point
   */
  void setWithCenter(const Vector2f &point);
  /**
   * \brief AABB Collision detection with another box
   */
  bool
  checkCollision(Box other,
                 BoxCollisionCheck boxCollisionCheck = BoxCollisionCheck::All);

  /**
   * \brief Output data to console
   */
  void print();
  slideOut slide(const Vector2f &move, const std::vector<Box *> &solids,
                 bool pushOut = true, bool useMoveBox = true);

  operator SDL_Rect() const {
    return SDL_Rect(position.x, position.y, size.x, size.y);
  }

  Vector2f position;
  Vector2f size;
};
