#pragma once
#include "Box.hpp"
#include "Math.hpp"
#include "Vector2f.hpp"

class Camera {
public:
  static void setPosition(const Vector2f &otherPosition);

  /**
   * \brief Gets the center position of the cameras box
   */
  static Vector2f getPosition();

  /**
   * \brief Gets the zoom of the camera
   */
  static float getScale();

  /**
   * \brief Gets the camera as a box, uses the scale & screen sizing
   */
  static Box getBox();

  /**
   * \brief Fully resets the camera
   */
  static void resetCamera();

  static Box cameraLimitBox;
  static float scale;

  // private:
  static Vector2f position;
};
