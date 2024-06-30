#include "Light.h"

void Light::update(float deltaTime) {
  if (image.path == "")
    return;

  SDL_Texture *lightTexture =
      ResourceManager::getInstance(GameManager::renderer)
          .getColoredTexture({r, g, b}, image.path);

  SDL_SetTextureBlendMode(lightTexture, SDL_BLENDMODE_ADD);
  SDL_Rect l = getRenderBox(entity);
  SDL_SetRenderTarget(GameManager::renderer,
                      GameManager::shadowFilter->shadowFilter);
  SDL_RenderCopy(GameManager::renderer, lightTexture, nullptr, &l);
  SDL_SetRenderTarget(GameManager::renderer, nullptr);
}
