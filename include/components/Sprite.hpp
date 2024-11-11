#pragma once

#include "Angle.hpp"
#include "Animation.hpp"
#include "GameManager.hpp"
#include "Image.hpp"
#include "Math.hpp"
#include "SDL_blendmode.h"
#include <SDL.h>

class Animation;
class Entity;

/**
 * \brief Components made for 2D rendering (uses entity->useLayer) which makes
 * it sort in the update loop
 */
class Sprite : public Component {
public:
  Sprite(Entity &entity) : Component(entity) {}
  void update() override;

  /**
   * \brief Sets the texture
   * \param image The filepath
   * \param setSize Default is true and when true it will change the entityes
   * box to the images size so it doesn't strech \param keepCentered Only
   * matters if setSize is true, keeps the center of the entityBox when resizing
   * (uses setScale)
   */

  void loadTexture(const std::string &image, bool setSize = true,
                   bool keepCentered = true);
  /**
   * \brief Adds an animation to the animations map
   * \param title The title of the animation to go into the map
   * \param frames A vector of filepath strings for each frame of the animation
   */
  void addAnimation(std::string title, std::vector<Image> frames,
                    float fps);

  void setAlpha(Uint8 alpha);

  Image image;

  /**
   * \brief Will show the entityBox as a green box (for debuging)
   */
  bool showBorders = false;
  /**
   * \brief Allows for rotation
   */
  Angle angle;
  /**
   * \brief The animations
   */
  std::unordered_map<std::string, Animation *> animations;
  /**
   * \brief The update loop will set this to true if it was within the screens
   * bounds and rendered the prevous frame
   */
  bool onScreen = false;
  /**
   * \brief SDL Flips
   */

  bool preventWeirdBorder = true;
};
