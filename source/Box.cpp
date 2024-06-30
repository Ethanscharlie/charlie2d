#include "Box.h"

float Box::getRight() { return position.x + size.x; }
float Box::getLeft() { return position.x; }
float Box::getTop() { return position.y; }
float Box::getBottom() { return position.y + size.y; }

Vector2f Box::getTopLeftCorner() { return position; }
Vector2f Box::getTopMiddle() { return {position.x + size.x / 2, position.y}; }
Vector2f Box::getTopRightCorner() { return {getRight(), position.y}; }
Vector2f Box::getBottomLeftCorner() { return {position.x, getBottom()}; }
Vector2f Box::getBottomMiddle() {
  return {position.x + size.x / 2, getBottom()};
}
Vector2f Box::getMiddleLeft() { return {position.x, position.y + size.y / 2}; }
Vector2f Box::getMiddleRight() { return {getRight(), position.y + size.y / 2}; }
Vector2f Box::getBottomRightCorner() { return {getRight(), getBottom()}; }
Vector2f Box::getCenter() {
  return {position.x + size.x / 2, position.y + size.y / 2};
}

void Box::setScale(const Vector2f &point) {
  Vector2f center = getCenter();
  size = (point);
  setWithCenter(center);
}

void Box::changeScale(const Vector2f &change) {
  Vector2f center = getCenter();
  size = (change);
  setWithCenter(center);
}

void Box::setWithCenter(const Vector2f &point) {
  position.x = point.x - size.x / 2;
  position.y = point.y - size.y / 2;
}

bool Box::checkCollision(Box other, BoxCollisionCheck boxCollisionCheck) {

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

void Box::print() {
  std::cout << position.x << ", " << position.y << ", " << size.x << ", "
            << size.y << std::endl;
}

slideOut Box::slide(const Vector2f &move, const std::vector<Box *> &solids,
                    bool pushOut, bool useMoveBox) {
  slideOut out;

  // Start
  Vector2f movement = move;

  // This system checks collisions for horizontal and vericle movment
  // seperatly If there is a collsion the entityBox can be moved in the
  // opposite direction outside the collided box
  // Collsions are restarted if there is any colllsion at all to prevent
  // clipping

  // Create Horizonal check box
  // The check box is sized to the move to prevent clipping
  // at high speeds and low framerates
  position += {movement.x, 0};

  // Horizontal check and move
HORREDO:
  for (Box *col : solids) {
    if (!checkCollision(*col))
      continue;

    out.hitList.push_back(col);
    if (!pushOut)
      continue;

    Box *other = col;
    if (movement.x > 0) {
      position.x = (other->getLeft() - size.x - 0.0001);
      out.horizontalHit = true;
    } else if (movement.x < 0) {
      position.x = (other->getRight());
      out.horizontalHit = true;
    }

    // goto HORREDO;
  }

  // Create verticle check box
  position += {0, movement.y};

  // Verticle collision check and move
VERREDO:
  for (Box *col : solids) {
    if (col->position.x == 0 && col->position.y == 0 && col->size.x == 0 &&
        col->size.y == 0)
      continue;
    if (!checkCollision(*col))
      continue;

    out.hitList.push_back(col);
    if (!pushOut)
      continue;

    Box *other = col;
    if (movement.y > 0) {
      position.y = (other->getTop() - size.y - 0.0001);
      out.verticleHit = true;
    } else if (movement.y < 0) {
      position.y = other->getBottom();
      out.verticleHit = true;
    }

    // goto VERREDO;
  }

  return out;
}
