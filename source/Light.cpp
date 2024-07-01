#include "Light.h"
#include "Math.h"
#include "SDL_blendmode.h"

void Light::update(float deltaTime) {
  if (image.path == "")
    return;

  image.color = {r, g, b};
  image.blendMode = SDL_BLENDMODE_ADD;

  SDL_SetRenderTarget(GameManager::renderer,
                      GameManager::shadowFilter->shadowFilter);
  image.render(getRenderBox(entity));
  SDL_SetRenderTarget(GameManager::renderer, nullptr);
}
