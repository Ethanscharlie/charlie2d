#pragma once

#include "Animation.h"
#include "GameManager.h"
#include "Math.h"
#include "SDL_blendmode.h"
#include "Serializer.h"
#include <SDL.h>
#include "Angle.h"

class Animation;
class Entity;

/**
 * \brief Components made for 2D rendering (uses entity->useLayer) which makes
 * it sort in the update loop
 */
class Sprite : public Component {
public:
  Sprite() {
    propertyRegister = {
        GET_PROP(image),
        GET_PROP(showBorders),
        GET_PROP(alpha),
        GET_PROP(angle),
        GET_PROP(renderAsUI),
        GET_PROP(flip),
        GET_PROP(preventWeirdBorder),
    };
    typeIsRendering = true;
  };

  void start() override { entity->useLayer = true; }

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
   * \brief Makes it render in UI positioning (Use for images as UI)
   */
  bool renderAsUI = false;
  /**
   * \brief SDL Flips
   */
  SDL_RendererFlip flip = SDL_FLIP_NONE;

  SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND;

  Uint8 alpha = 255;

  bool preventWeirdBorder = true;
};
REGISTER_COMPONENT_TYPE(Sprite);
