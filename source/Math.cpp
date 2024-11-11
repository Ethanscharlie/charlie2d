#include "Math.hpp"

#include "Box.hpp"
#include "Camera.hpp"
#include "Entity.hpp"
#include "GameManager.hpp"
#include "Image.hpp"
#include "Vector2f.hpp"
#include <iostream>
#include <math.h>
#include <random>
#include <sstream>

auto randFloat(float min, float max) -> float {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(min, max);
  return dis(gen);
}

auto floatToString(float value) -> std::string {
  std::stringstream ss;
  ss << value;
  return ss.str();
}

auto getLogcialPosition(Vector2f screenPos) -> Vector2f {
  int virtualWidth = 0, virtualHeight = 0;
  SDL_RenderGetLogicalSize(GameManager::getRenderer(), &virtualWidth,
                           &virtualHeight);

  int windowWidth = 0, windowHeight = 0;
  SDL_GetWindowSize(GameManager::getWindow(), &windowWidth, &windowHeight);

  double scale = NAN;
  int xOffset = 0, yOffset = 0;
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

auto getScreenPosition(Vector2f logicalPos) -> Vector2f {
  int virtualWidth = 0, virtualHeight = 0;
  SDL_RenderGetLogicalSize(GameManager::getRenderer(), &virtualWidth,
                           &virtualHeight);

  int windowWidth = 0, windowHeight = 0;
  SDL_GetWindowSize(GameManager::getWindow(), &windowWidth, &windowHeight);

  double scale = NAN;
  int xOffset = 0, yOffset = 0;
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

auto getImGuiPosition(Vector2f pos) -> Vector2f {
  Vector2f out;

  SDL_Rect logicalRect = getLogicalRect();
  Vector2f logicalPos = {(float)logicalRect.x, (float)logicalRect.y};

  out = pos + GameManager::getGameWindowSize() / 2;
  return out;
}

auto getLogicalRect() -> SDL_Rect {
  int logical_w = 1, logical_h = 1;
  int output_w = GameManager::getCurrentWindowSize().x;
  int output_h = GameManager::getCurrentWindowSize().y;
  float want_aspect = 1.0f;
  float real_aspect = 1.0f;
  float scale = NAN;
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

auto getWindowPosition(Vector2f gamePosition) -> Vector2f {
  int widthInPixels = 0, heightInPixels = 0;
  int widthInPoints = 0, heightInPoints = 0;
  SDL_GetWindowSize(GameManager::getWindow(), &widthInPoints, &heightInPoints);
  SDL_GL_GetDrawableSize(GameManager::getWindow(), &widthInPixels, &heightInPixels);
  float dpiScaleX = widthInPixels / (float)widthInPoints;
  float dpiScaleY = heightInPixels / (float)heightInPoints;

  SDL_Rect logicalDst = getLogicalRect();
  return {gamePosition.x * (logicalDst.w / GameManager::getGameWindowSize().x) +
              logicalDst.x,
          gamePosition.y * (logicalDst.h / GameManager::getGameWindowSize().y) +
              logicalDst.y};
}

auto getFullLogicalToWindowPosition(Vector2f fullLogicalPosition) -> Vector2f {
  return fullLogicalPosition *
         (GameManager::getCurrentWindowSize() / GameManager::getGameWindowSize());
}

auto getTypeNameWithoutNumbers(std::type_index typeIndex) -> std::string {
  std::string name = typeIndex.name();
  std::size_t pos = 0;

  while (pos < name.length() && std::isdigit(name[pos])) {
    ++pos;
  }

  name = name.substr(pos);
  return name;
}

auto getRenderBox(Box box, EntityRenderPositionType renderPositionType) -> Box {
  Box renderBox;

  switch (renderPositionType) {
  case EntityRenderPositionType::World:
    renderBox.position =
        (box.position - Camera::getPosition()) * Camera::getScale() +
        GameManager::getGameWindowSize() / 2;
    renderBox.size = box.size * Camera::getScale();
    break;

  case EntityRenderPositionType::Screen:
    renderBox.position = box.position + GameManager::getGameWindowSize() / 2;
    renderBox.size = box.size;

    break;
  }

  return renderBox;
}

auto getRenderBox(Entity *entity) -> Box {
  Box box = entity->box;
  return getRenderBox(box, entity->renderPositionType);
}

auto hexToRGB(int hexColor) -> std::array<Uint8, 3> {
  Uint8 red = (hexColor >> 16) & 0xFF;
  Uint8 green = (hexColor >> 8) & 0xFF;
  Uint8 blue = hexColor & 0xFF;
  return {red, green, blue};
}

std::vector<Image> generateSpritesheetAnimation(Image image, int gridSize,
                                                int row) {
  int width, height;
  SDL_QueryTexture(image.texture, nullptr, nullptr, &width, &height);

  int gridWidth = width / gridSize;
  int gridHeight = height / gridSize;

  if (gridHeight < row) {
    throw std::runtime_error("Row was to high in animation");
  }

  std::vector<Image> frames;
  int y = row * gridSize;
  for (int x = 0; x < gridSize * gridWidth; x += gridSize) {
    Image frame = {"res/images/birds.png"};
    frame.srcRect = {x, y, gridSize, gridSize};
    frames.push_back(frame);
  }

  return frames;
}
