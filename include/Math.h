#pragma once

#include "Vector2f.h"
#include <SDL.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>

/** @file */

struct Angle {
public:
  Angle() {}

  /**
   * \brief Rotates to be pointing at something
   * \param center Should be the position of the entity to be rotated
   * \param point What are you lookin at?!
   */
  void lookAt(const Vector2f center, const Vector2f &point) {
    setWithVector({point.x - center.x, point.y - center.y});
  }

  /**
   * \brief Gets a direction vector (Good for directional movement)
   * \return A normalized vector of this angle
   */
  Vector2f getVector() {
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
  void setWithVector(Vector2f vector) {
    radians = std::atan2(vector.y, vector.x);
  }

  /**
   * \brief Rotates in degrees
   * \param angle float for degrees
   */
  void rotate(float angle) {
    radians += angle * (180.0 / 3.141592653589793238463);
  }

  float radians = 0;
};

/**
 * \brief Two Vector2f s put together
 *
 * Boxes are simple datatypes composed of two vectors, a position (top left
 * corner) and size
 *
 * Don't mix these up with entityBox (used for parent based positioning)
 */
struct Box {
  Box() {}

  Box(float p_x, float p_y, float p_w, float p_h)
      : position(p_x, p_y), size(p_w, p_h) {}

  /**
   * \brief Gets the X position for the right side
   */
  float getRight() { return position.x + size.x; }
  /**
   * \brief Gets the X position for the left side
   */
  float getLeft() { return position.x; }
  /**
   * \brief Gets the Y position for the Top side
   */
  float getTop() { return position.y; }
  /**
   * \brief Gets the Y position for the Bottom side
   */
  float getBottom() { return position.y + size.y; }

  Vector2f getTopLeftCorner() { return position; }
  Vector2f getTopMiddle() { return {position.x + size.x / 2, position.y}; }
  Vector2f getTopRightCorner() { return {getRight(), position.y}; }
  Vector2f getBottomLeftCorner() { return {position.x, getBottom()}; }
  Vector2f getBottomMiddle() { return {position.x + size.x / 2, getBottom()}; }
  Vector2f getBottomRightCorner() { return {getRight(), getBottom()}; }
  Vector2f getCenter() {
    return {position.x + size.x / 2, position.y + size.y / 2};
  }

  /**
   * \brief Sets the size but also moves the object to still be centered in the
   * same position
   */
  void setScale(const Vector2f &point) {
    Vector2f center = getCenter();
    size = (point);
    setWithCenter(center);
  }

  /**
   * \brief Changes the size (Adds) but also moves the object to still be
   * centered in the same position
   */
  void changeScale(const Vector2f &change) {
    Vector2f center = getCenter();
    size = (change);
    setWithCenter(center);
  }

  /**
   * \brief Sets the center of the box to the point
   */
  void setWithCenter(const Vector2f &point) {
    position.x = point.x - size.x / 2;
    position.y = point.y - size.y / 2;
  }

  /**
   * \brief AABB Collision detection with another box
   */
  bool checkCollision(Box other) {
    return getLeft() < other.getRight() && getRight() > other.getLeft() &&
           getTop() < other.getBottom() && getBottom() > other.getTop();
  }

  /**
   * \brief Output data to console
   */
  void print() {
    std::cout << position.x << ", " << position.y << ", " << size.x << ", "
              << size.y << std::endl;
  }

  Vector2f position;
  Vector2f size;
};

/**
 * \brief Random floating point number
 */
inline float randFloat(float min, float max) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(min, max);
  return dis(gen);
}

/**
 * \brief Grabs a random element from an array
 */
template <typename T, std::size_t N> T getRandomElement(const T (&arr)[N]) {
  // Generate a random index within the array bounds
  std::size_t randomIndex = std::rand() % N;

  // Return the randomly selected element
  return arr[randomIndex];
}

// // Function to check if a line (start + direction * t) intersects with a box
// bool doesLineIntersect(const Vector2f& start, const Vector2f& direction,
// const Box& box) {
//     float tmin = 0.0f;
//     float tmax = 1.0f;
//
//     for (int i = 0; i < 2; ++i) {
//         float invDir = 1.0f / direction.x; // Assuming direction.x != 0
//         float t0 = (box.position.x - start.x) * invDir;
//         float t1 = (box.position.x + box.size.x - start.x) * invDir;
//
//         if (invDir < 0.0f)
//             std::swap(t0, t1);
//
//         tmin = std::max(tmin, t0);
//         tmax = std::min(tmax, t1);
//
//         if (tmin > tmax)
//             return false;
//
//         float invDirY = 1.0f / direction.y; // Assuming direction.y != 0
//         float ty0 = (box.position.y - start.y) * invDirY;
//         float ty1 = (box.position.y + box.size.y - start.y) * invDirY;
//
//         if (invDirY < 0.0f)
//             std::swap(ty0, ty1);
//
//         float tymin = std::max(tmin, ty0);
//         float tymax = std::min(tmax, ty1);
//
//         if (tymin > tymax || tmin > tymax || tymin > tmax)
//             return false;
//     }
//
//     return true;
// }
