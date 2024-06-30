#pragma once
#include "Vector2f.h"

struct Box;

struct Circle {
  Circle(Vector2f _position, float _radius)
      : position(_position), radius(_radius) {}

  Circle() : position({0, 0}), radius(0.0f) {}

  bool checkCollision(const Circle &other);

  bool checkCollision(Box box);

  Vector2f position;
  float radius;
};
