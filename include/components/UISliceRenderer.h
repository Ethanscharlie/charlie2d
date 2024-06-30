#pragma once
#include "Camera.h"
#include "Entity.h"
#include "GameManager.h"
#include "ResourceManager.h"
#include "SDL_stdinc.h"
#include "Serializer.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

/**
 * \brief Renders a texture without stretching the corners
 * Don't forget to use a UICanvas
 */
class UISliceRenderer : public Component {
public:
  UISliceRenderer() {
    propertyRegister = {
        GET_PROP(rendererInWorld),
        GET_PROP(alpha),
    };

    loadTexture(textureName);
    typeIsRendering = true;
  };

  void setColor(std::array<Uint8, 3> color);
  void start() override;
  /**
   * \brief Loads a texture from filepath, this is meant to be square for a
   * panel of some sort
   */
  void loadTexture(std::string name);
  void update(float deltaTime) override;

  void Draw9SlicedTexture(SDL_Renderer *renderer, SDL_Texture *texture,
                          SDL_Rect destRect, int borderSize);

  bool rendererInWorld = false;
  Uint8 alpha = 255;

private:
  SDL_Texture *texture;
  std::string textureName = "img/panel.png";
};
REGISTER_COMPONENT_TYPE(UISliceRenderer);
