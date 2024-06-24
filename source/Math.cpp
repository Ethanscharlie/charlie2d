#include "Math.h"
#include "Camera.h"
#include "Vector2f.h"
#include <cmath>

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

#include "GameManager.h"
#include "ResourceManager.h"

Image::Image(std::string _path) : path(_path) {
  texture =
      ResourceManager::getInstance(GameManager::renderer).getTexture(path);
}

Vector2f getLogcialPosition(Vector2f screenPos) {
  int virtualWidth, virtualHeight;
  SDL_RenderGetLogicalSize(GameManager::renderer, &virtualWidth,
                           &virtualHeight);

  int windowWidth, windowHeight;
  SDL_GetWindowSize(GameManager::window, &windowWidth, &windowHeight);

  double scale;
  int xOffset, yOffset;
  if (windowWidth * virtualHeight > windowHeight * virtualWidth) {
    // Calculate the scale based on height
    scale = (double)windowHeight / virtualHeight;
    xOffset = (windowWidth - virtualWidth * scale) / 2;
    yOffset = 0;
  } else {
    // Calculate the scale based on width
    scale = (double)windowWidth / virtualWidth;
    xOffset = 0;
    yOffset = (windowHeight - virtualHeight * scale) / 2;
  }

  float logicalX = 1.0f * (screenPos.x - xOffset) / scale;
  float logicalY = 1.0f * (screenPos.y - yOffset) / scale;

  return {logicalX, logicalY};
}

Vector2f getScreenPosition(Vector2f logicalPos) {
  int virtualWidth, virtualHeight;
  SDL_RenderGetLogicalSize(GameManager::renderer, &virtualWidth,
                           &virtualHeight);

  int windowWidth, windowHeight;
  SDL_GetWindowSize(GameManager::window, &windowWidth, &windowHeight);

  double scale;
  int xOffset, yOffset;
  if (windowWidth * virtualHeight > windowHeight * virtualWidth) {
    // Calculate the scale based on height
    scale = (double)windowHeight / virtualHeight;
    xOffset = (windowWidth - virtualWidth * scale) / 2;
    yOffset = 0;
  } else {
    // Calculate the scale based on width
    scale = (double)windowWidth / virtualWidth;
    xOffset = 0;
    yOffset = (windowHeight - virtualHeight * scale) / 2;
  }

  float screenX = logicalPos.x * scale + xOffset;
  float screenY = logicalPos.y * scale + yOffset;

  return {screenX, screenY};
}

Vector2f getImGuiPosition(Vector2f pos) {
  Vector2f out;

  SDL_Rect logicalRect = getLogicalRect();
  Vector2f logicalPos = {(float)logicalRect.x, (float)logicalRect.y};

  out = pos + GameManager::gameWindowSize / 2;
  return out;
}

#include "Entity.h"
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

SDL_Rect getLogicalRect() {
  int logical_w = 1, logical_h = 1;
  int output_w = GameManager::currentWindowSize.x;
  int output_h = GameManager::currentWindowSize.y;
  float want_aspect = 1.0f;
  float real_aspect = 1.0f;
  float scale;
  SDL_Rect logical_src_rect;
  SDL_Rect logical_dst_rect;

  want_aspect = (float)logical_w / logical_h;
  real_aspect = (float)output_w / output_h;

  logical_src_rect.x = 0.0f;
  logical_src_rect.y = 0.0f;
  logical_src_rect.w = (float)logical_w;
  logical_src_rect.h = (float)logical_h;

  if (want_aspect > real_aspect) {
    /* We want a wider aspect ratio than is available - letterbox it */
    scale = (float)output_w / logical_w;
    logical_dst_rect.x = 0.0f;
    logical_dst_rect.w = (float)output_w;
    logical_dst_rect.h = SDL_floorf(logical_h * scale);
    logical_dst_rect.y = (output_h - logical_dst_rect.h) / 2.0f;
  } else {
    /* We want a narrower aspect ratio than is available - use side-bars */
    scale = (float)output_h / logical_h;
    logical_dst_rect.y = 0.0f;
    logical_dst_rect.h = (float)output_h;
    logical_dst_rect.w = SDL_floorf(logical_w * scale);
    logical_dst_rect.x = (output_w - logical_dst_rect.w) / 2.0f;
  }

  return logical_dst_rect;
}

Vector2f getWindowPosition(Vector2f gamePosition) {
  int widthInPixels, heightInPixels;
  int widthInPoints, heightInPoints;
  SDL_GetWindowSize(GameManager::window, &widthInPoints, &heightInPoints);
  SDL_GL_GetDrawableSize(GameManager::window, &widthInPixels, &heightInPixels);
  float dpiScaleX = widthInPixels / (float)widthInPoints;
  float dpiScaleY = heightInPixels / (float)heightInPoints;

  SDL_Rect logicalDst = getLogicalRect();
  return {gamePosition.x * (logicalDst.w / GameManager::gameWindowSize.x) +
              logicalDst.x,
          gamePosition.y * (logicalDst.h / GameManager::gameWindowSize.y) +
              logicalDst.y};
}

Vector2f getFullLogicalToWindowPosition(Vector2f fullLogicalPosition) {
  return fullLogicalPosition *
         (GameManager::currentWindowSize / GameManager::gameWindowSize);
}

std::string getTypeNameWithoutNumbers(std::type_index typeIndex) {
  std::string name = typeIndex.name();
  std::size_t pos = 0;

  while (pos < name.length() && std::isdigit(name[pos])) {
    ++pos;
  }

  name = name.substr(pos);
  return name;
}

Box getRenderBox(Entity *entity) {
  Box box = entity->box;
  Box renderBox;

  switch (entity->renderPositionType) {
  case EntityRenderPositionType::World:
    renderBox.position =
        (box.position - Camera::getPosition()) * Camera::getScale() +
        GameManager::gameWindowSize / 2;
    renderBox.size = box.size * Camera::getScale();
    break;

  case EntityRenderPositionType::Screen:
    renderBox.position = box.position + GameManager::gameWindowSize / 2;
    renderBox.size = box.size;

    break;
  }

  return renderBox;
}
