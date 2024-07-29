#include "Angle.hpp"
void Angle::lookAt(const Vector2f center, const Vector2f &point) {
  setWithVector({point.x - center.x, point.y - center.y});
}

/**
 * \brief Gets a direction vector (Good for directional movement)
 * \return A normalized vector of this angle
 */
auto Angle::getVector() -> Vector2f {
  Vector2f vector = {std::cos(radians), std::sin(radians)};

  float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
  if (length > 0) {
    vector.x = vector.x / length;
    vector.y = vector.y / length;
  } else {
    vector = {0.0f, 0.0f};
  }

  return vector;
}

/**
 * \brief Sets the angle with a vector (Doesn't need to be normalized)
 */
void Angle::setWithVector(Vector2f vector) {
  radians = std::atan2(vector.y, vector.x);
}

/**
 * \brief Rotates in degrees
 * \param angle float for degrees
 */
void Angle::rotate(float degrees) {
  radians += degrees * (M_PI / 180.0);
  radians = fmod(radians, 2 * M_PI);
  if (radians < 0) {
    radians += 2 * M_PI;
  }
}
