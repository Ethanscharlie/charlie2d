#pragma once

#include "Component.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "Vector2f.h"
#include <SDL.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <typeindex>

/** @file */

struct Image {
  Image() {
    texture = nullptr;
    path = "";
  }
  Image(std::string _path);
  Image(SDL_Texture *_texture) : path(""), texture(_texture) {}
  SDL_Texture *texture;
  std::string path;
};

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
  void rotate(float degrees) {
    radians += degrees * (M_PI / 180.0);
    radians = fmod(radians, 2 * M_PI);
    if (radians < 0) {
      radians += 2 * M_PI;
    }
  }

  float radians = 0;
};

class Entity;
struct Box;
struct slideOut {
  std::vector<Box *> hitList;
  bool horizontalHit = false;
  bool verticleHit = false;
};

enum class BoxCollisionCheck { All, VerticaleOnly, HorizontalOnly };

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
  Vector2f getMiddleLeft() { return {position.x, position.y + size.y / 2}; }
  Vector2f getMiddleRight() { return {getRight(), position.y + size.y / 2}; }
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
  bool
  checkCollision(Box other,
                 BoxCollisionCheck boxCollisionCheck = BoxCollisionCheck::All) {

    switch (boxCollisionCheck) {
    case BoxCollisionCheck::All:
      break;

    case BoxCollisionCheck::HorizontalOnly:
      return getLeft() < other.getRight() && getRight() > other.getLeft();

    case BoxCollisionCheck::VerticaleOnly:
      return getTop() < other.getBottom() && getBottom() > other.getTop();
    }

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

  slideOut slide(const Vector2f &move, const std::vector<Box *> &solids,
                 bool pushOut = true, bool useMoveBox = true);

  operator SDL_Rect() const {
    return SDL_Rect(position.x, position.y, size.x, size.y);
  }

  Vector2f position;
  Vector2f size;
};

struct Circle {
  Circle(Vector2f _position, float _radius)
      : position(_position), radius(_radius) {}

  Circle() : position({0, 0}), radius(0.0f) {}

  bool checkCollision(const Circle &other) {
    float distance = std::sqrt(std::pow(other.position.x - position.x, 2) +
                               std::pow(other.position.y - position.y, 2));

    return distance <= radius + other.radius;
  }

  Vector2f position;
  float radius;
};

bool checkCollision(Box box, Circle circle);

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

/**
 * \brief Converts a float to a string without unnessessary zeros
 */
std::string floatToString(float value);

/**
 * \brief Gets a screen position as a logical position
 */
Vector2f getLogcialPosition(Vector2f screenPos);
/**
 * \brief Gets a logical position as a screen position
 */
Vector2f getScreenPosition(Vector2f logicalPos);
/**
 * \brief Should be used for all imgui frames, to use please render imgui
 * without logical rendering first and then turn it back on
 */
Vector2f getImGuiPosition(Vector2f pos);

/**
 * \brief Gets sdl2's logical rect because sdl doesn't offer a way too (Ur
 * Welcome)
 */

SDL_Rect getLogicalRect();

Vector2f getWindowPosition(Vector2f gamePosition);

Vector2f getFullLogicalToWindowPosition(Vector2f fullLogicalPosition);

std::string getTypeNameWithoutNumbers(std::type_index typeIndex);

Box getRenderBox(Entity *entity);
