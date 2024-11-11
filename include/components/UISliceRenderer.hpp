#pragma once
#include "Camera.hpp"
#include "Entity.hpp"
#include "GameManager.hpp"
#include "SDL_stdinc.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Image.hpp"

/**
 * \brief Renders a texture without stretching the corners
 * Don't forget to use a UICanvas
 */
class UISliceRenderer : public Component {
public:
  UISliceRenderer(Entity &entity) : Component(entity) {}
  void setColor(std::array<Uint8, 3> color);
  /**
   * \brief Loads a texture from filepath, this is meant to be square for a
   * panel of some sort
   */
  void loadTexture(std::string name);
  void update() override;

  void Draw9SlicedTexture(SDL_Renderer *renderer, SDL_Texture *texture,
                          SDL_Rect destRect, int borderSize);

  bool rendererInWorld = false;

private:
  Image image;
};
