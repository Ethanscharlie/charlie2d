#include "Math.h"

bool checkCollision(Box box, Circle circle) {
  // Calculate the closest point to the circle within the rectangle
  float closestX =
      std::max(box.getLeft(), std::min(circle.position.x, box.getRight()));
  float closestY =
      std::max(box.getTop(), std::min(circle.position.y, box.getBottom()));

  // Calculate the distance between the circle's center and this closest point
  float distanceX = circle.position.x - closestX;
  float distanceY = circle.position.y - closestY;
  float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);

  // Check if the distance is less than the circle's radius squared
  return distanceSquared < (circle.radius * circle.radius);
}

std::string floatToString(float value) {
  std::stringstream ss;
  ss << value;
  return ss.str();
}
