#pragma once

#include "Animation.h"
#include "Collider.h"
#include "GameManager.h"
#include "Math.h"
#include <SDL.h>

class Animation;
class Entity;

/**
 * \brief Components made for 2D rendering (uses entity->useLayer) which makes
 * it sort in the update loop
 */
class Sprite : public Component {
public:
  Sprite() : Component("sprite"){};

  void start() override {
    entity->useLayer = true;
    typeIsRendering = true;
  }

  void update(float deltaTime) override;

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
  void addAnimation(std::string title, std::vector<std::string> frames,
                    float fps);


  void setAlpha(Uint8 alpha);

  /**
   * \brief Allows you to grab only a part of an image
   */
  SDL_Rect sourceRect = {0, 0, 0, 0};
  SDL_Texture *texture = nullptr;
  /**
   * \brief The filename of the loaded texture
   */
  std::string textureName = "";
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
   * \brief Makes it render in UI positioning (Use for images as UI)
   */
  bool renderAsUI = false;
  /**
   * \brief SDL Flips
   */
  SDL_RendererFlip flip = SDL_FLIP_NONE;

  Uint8 alpha = 255;

  bool preventWeirdBorder = true;
};
