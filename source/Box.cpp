#include "Box.h"

auto Box::getRight() -> float { return position.x + size.x; }
auto Box::getLeft() -> float { return position.x; }
auto Box::getTop() -> float { return position.y; }
auto Box::getBottom() -> float { return position.y + size.y; }

auto Box::getTopLeftCorner() -> Vector2f { return position; }
auto Box::getTopMiddle() -> Vector2f { return {position.x + size.x / 2, position.y}; }
auto Box::getTopRightCorner() -> Vector2f { return {getRight(), position.y}; }
auto Box::getBottomLeftCorner() -> Vector2f { return {position.x, getBottom()}; }
auto Box::getBottomMiddle() -> Vector2f {
  return {position.x + size.x / 2, getBottom()};
}
auto Box::getMiddleLeft() -> Vector2f { return {position.x, position.y + size.y / 2}; }
auto Box::getMiddleRight() -> Vector2f { return {getRight(), position.y + size.y / 2}; }
auto Box::getBottomRightCorner() -> Vector2f { return {getRight(), getBottom()}; }
auto Box::getCenter() -> Vector2f {
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

auto Box::checkCollision(Box other, BoxCollisionCheck boxCollisionCheck) -> bool {

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

auto Box::slide(const Vector2f &move, const std::vector<Box *> &solids,
                    bool pushOut, bool useMoveBox) -> slideOut {
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
