#include "Circle.h"
#include "Box.h"

bool Circle::checkCollision(Box box) {
  // Calculate the closest point to the circle within the rectangle
  float closestX =
      std::max(box.getLeft(), std::min(position.x, box.getRight()));
  float closestY =
      std::max(box.getTop(), std::min(position.y, box.getBottom()));

  // Calculate the distance between the s center and this closest point
  float distanceX = position.x - closestX;
  float distanceY = position.y - closestY;
  float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);

  // Check if the distance is less than the s radius squared
  return distanceSquared < (radius * radius);
}

bool Circle::checkCollision(const Circle &other) {
  float distance = std::sqrt(std::pow(other.position.x - position.x, 2) +
                             std::pow(other.position.y - position.y, 2));

  return distance <= radius + other.radius;
}
