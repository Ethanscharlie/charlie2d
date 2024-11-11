#include "Light.hpp"
#include "Math.hpp"
#include "SDL_blendmode.h"
#include "ShadowFilter.hpp"

void Light::update() {
  if (image.path == "")
    return;

  image.color = {r, g, b};
  image.blendMode = SDL_BLENDMODE_ADD;

  SDL_SetRenderTarget(
      GameManager::getRenderer(),
      GameManager::getComponents<ShadowFilter>()[0]->shadowFilter);
  image.render(getRenderBox(&entity));
  SDL_SetRenderTarget(GameManager::getRenderer(), nullptr);
}
