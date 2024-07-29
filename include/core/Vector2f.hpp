#pragma once

#include <cmath>
#include <iostream>
/**
 * \brief A simple Vector2 float class, with an x & y property that supports
 * math with floats and other Vectors
 */
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

  bool operator==(Vector2f other) { return x == other.x && y == other.y; }
  bool operator!=(Vector2f other) { return x != other.x || y != other.y; }

  Vector2f operator+(const float num) { return Vector2f(x + num, y + num); }

  Vector2f operator-(const float num) { return Vector2f(x - num, y - num); }
  Vector2f operator*(const float num) { return Vector2f(x * num, y * num); }
  Vector2f operator/(const float num) { return Vector2f(x / num, y / num); }

  /**
   * \brief Changes itself to be in terms of one (Useful for Angle movement)
   */
  Vector2f &normalize() {
    if (length() == 0)
      return *this;
    *this *= (1.0 / length());
    return *this;
  }

  /**
   * \brief Length of the vector (As a line)
   */
  float length() const { return std::sqrt(x * x + y * y); }

  /**
   * \brief Display to console
   */
  void print() { std::cout << x << ", " << y << std::endl; }

  /**
   * \brief Distance between its position and another position
   */
  float dist(Vector2f point) {
    return sqrt(pow(point.x - x, 2) + pow(point.y - y, 2));
  }

  float x, y;
};
