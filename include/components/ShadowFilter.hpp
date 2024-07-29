#pragma once
#include "Component.hpp"
#include "GameManager.hpp"
#include "SDL_rect.h"
#include "SDL_render.h"

class ShadowFilter : public Component {
public:
  void start() override {
    shadowFilter = SDL_CreateTexture(
        GameManager::renderer, SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET, GameManager::gameWindowSize.x,
        GameManager::gameWindowSize.y);
    SDL_SetTextureBlendMode(shadowFilter, SDL_BLENDMODE_MOD);
  }

  void resetShadowFilter() {
    // Clear Shadow Filter
    SDL_Rect screenRect = {0, 0, (int)GameManager::gameWindowSize.x,
                           (int)GameManager::gameWindowSize.y};
    SDL_SetRenderTarget(GameManager::renderer, shadowFilter);
    SDL_RenderClear(GameManager::renderer);
    SDL_SetRenderDrawColor(GameManager::renderer, shadowFilterTint[0],
                           shadowFilterTint[1], shadowFilterTint[2], 255);
    SDL_RenderFillRect(GameManager::renderer, &screenRect);
    SDL_SetRenderTarget(GameManager::renderer, nullptr);
  }

  void update(float deltatime) override {
    SDL_Rect screenRect = {0, 0, (int)GameManager::gameWindowSize.x,
                           (int)GameManager::gameWindowSize.y};
    SDL_RenderCopy(GameManager::renderer, shadowFilter, nullptr, &screenRect);
  }

  std::array<Uint8, 3> shadowFilterTint = {255, 255, 255};
  SDL_Texture *shadowFilter;
};
