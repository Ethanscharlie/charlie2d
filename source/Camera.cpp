#include "Camera.h"
#include "GameManager.h"
#include "SDL_render.h"
#include "Vector2f.h"

Vector2f Camera::position = {0, 0};
Box Camera::cameraLimitBox;
float Camera::scale = 1;

void Camera::setPosition(const Vector2f &otherPosition) {
  // if (box.size.y / box.size.x !=
  //     currentWindowSize.x / currentWindowSize.y) {
  //   // currentWindowSize = box.size;
  // }
  position = otherPosition;

  if (cameraLimitBox.size.x == 0 && cameraLimitBox.size.y == 0)
    return;

  Box cameraBox = getBox();
  // cameraLimitBox.print();

  if (cameraBox.position.x < cameraLimitBox.position.x)
    cameraBox.position.x = cameraLimitBox.position.x;
  if (cameraBox.getRight() > cameraLimitBox.getRight())
    cameraBox.position.x = cameraLimitBox.getRight() - cameraBox.size.x;
  if (cameraBox.position.y < cameraLimitBox.position.y)
    cameraBox.position.y = cameraLimitBox.position.y;
  if (cameraBox.getBottom() > cameraLimitBox.getBottom())
    cameraBox.position.y = cameraLimitBox.getBottom() - cameraBox.size.y;

  position = cameraBox.getCenter();
}

Box Camera::getBox() {
  Box cameraBox;

  Vector2f logicalSize;
  cameraBox.size = (GameManager::gameWindowSize / scale);
  cameraBox.setWithCenter(position);
  return cameraBox;
}

Vector2f Camera::getPosition() { return position; }

float Camera::getScale() { return scale; }

void Camera::resetCamera() {
  cameraLimitBox = {0, 0, 0, 0};
  scale = 1;
  position = {0, 0};
}
