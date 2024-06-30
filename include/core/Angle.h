#pragma once
#include "Vector2f.h"

/**
 * \brief A simple class for manageing angles (used in Sprite)
 */
struct Angle {
public:
  Angle() {}

  /**
   * \brief Rotates to be pointing at something
   * \param center Should be the position of the entity to be rotated
   * \param point What are you lookin at?!
   */
  void lookAt(const Vector2f center, const Vector2f &point);
  /**
   * \brief Gets a direction vector (Good for directional movement)
   * \return A normalized vector of this angle
   */
  Vector2f getVector();
  /**
   * \brief Sets the angle with a vector (Doesn't need to be normalized)
   */
  void setWithVector(Vector2f vector);
  /**
   * \brief Rotates in degrees
   * \param angle float for degrees
   */
  void rotate(float degrees);

  float radians = 0;
};
