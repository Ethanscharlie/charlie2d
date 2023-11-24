#pragma once

#include <SDL.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>

struct Vector2f {
  Vector2f() : x(0.0f), y(0.0f) {}

  Vector2f(float p_x, float p_y) : x(p_x), y(p_y) {}

  Vector2f &operator=(const Vector2f &v) {
    x = v.x;
    y = v.y;
    return *this;
  }

  Vector2f operator+(const Vector2f &v) { return Vector2f(x + v.x, y + v.y); }
  Vector2f operator-(const Vector2f &v) { return Vector2f(x - v.x, y - v.y); }
  Vector2f operator*(const Vector2f &v) { return Vector2f(x * v.x, y * v.y); }
  Vector2f operator/(const Vector2f &v) { return Vector2f(x / v.x, y / v.y); }

  Vector2f &operator+=(const Vector2f &v) {
    x += v.x;
    y += v.y;
    return *this;
  }
  Vector2f &operator-=(Vector2f &v) {
    x -= v.x;
    y -= v.y;
    return *this;
  }
  Vector2f &operator*=(Vector2f &v) {
    x *= v.x;
    y *= v.y;
    return *this;
  }
  Vector2f &operator/=(Vector2f &v) {
    x /= v.x;
    y /= v.y;
    return *this;
  }

  Vector2f &operator+=(float s) {
    x += s;
    y += s;
    return *this;
  }
  Vector2f &operator-=(float s) {
    x -= s;
    y -= s;
    return *this;
  }
  Vector2f &operator*=(float s) {
    x *= s;
    y *= s;
    return *this;
  }
  Vector2f &operator/=(float s) {
    x /= s;
    y /= s;
    return *this;
  }

  Vector2f operator+(const float num) { return Vector2f(x + num, y + num); }

  Vector2f operator-(const float num) { return Vector2f(x - num, y - num); }
  Vector2f operator*(const float num) { return Vector2f(x * num, y * num); }
  Vector2f operator/(const float num) { return Vector2f(x / num, y / num); }

  Vector2f &normalize() {
    if (length() == 0)
      return *this;
    *this *= (1.0 / length());
    return *this;
  }

  float length() const { return std::sqrt(x * x + y * y); }

  void print() { std::cout << x << ", " << y << std::endl; }

  float dist(Vector2f point) {
    return sqrt(pow(point.x - x, 2) + pow(point.y - y, 2));
  }

  float x, y;
};

struct Angle {
public:
  Angle() {}

  void lookAt(const Vector2f center, const Vector2f &point) {
    setWithVector({point.x - center.x, point.y - center.y});
  }

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

  void setWithVector(Vector2f vector) {
    radians = std::atan2(vector.y, vector.x);
  }

  void rotate(float angle) {
    radians += angle * (180.0 / 3.141592653589793238463);
  }

  float radians = 0;
};

struct Box {
  Box()
  //: x(0.0f), y(0.0f)
  {}

  Box(float p_x, float p_y, float p_w, float p_h)
      : position(p_x, p_y), size(p_w, p_h) {}

  float getRight() { return position.x + size.x; }
  float getLeft() { return position.x; }
  float getTop() { return position.y; }
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

  void setScale(const Vector2f &point) {
    Vector2f center = getCenter();
    size = (point);
    setWithCenter(center);
  }
  void changeScale(const Vector2f &change) {
    Vector2f center = getCenter();
    size = (change);
    setWithCenter(center);
  }

  void setWithCenter(const Vector2f &point) {
    position.x = point.x - size.x / 2;
    position.y = point.y - size.y / 2;
  }

  bool checkCollision(Box other) {
    // if (entity->checkComponent<Sprite>() &&
    // !entity->getComponent<Sprite>().onScreen) {
    //   return false;
    // }

    return getLeft() < other.getRight() && getRight() > other.getLeft() &&
           getTop() < other.getBottom() && getBottom() > other.getTop();
  }

  void print() {
    std::cout << position.x << ", " << position.y << ", " << size.x << ", "
              << size.y << std::endl;
  }


  Vector2f position;
  Vector2f size;
};

inline float randFloat(float min, float max) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(min, max);
  return dis(gen);
}

template <typename T, std::size_t N> T getRandomElement(const T (&arr)[N]) {
  // Generate a random index within the array bounds
  std::size_t randomIndex = std::rand() % N;

  // Return the randomly selected element
  return arr[randomIndex];
}

// // Function to check if a line (start + direction * t) intersects with a box
// bool doesLineIntersect(const Vector2f& start, const Vector2f& direction, const Box& box) {
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
