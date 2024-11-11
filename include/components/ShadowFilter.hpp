#pragma once
#include "Component.hpp"
#include "Entity.hpp"
#include "GameManager.hpp"
#include "SDL_rect.h"
#include "SDL_render.h"

class ShadowFilter : public Component {
public:
  ShadowFilter(Entity& entity) : Component(entity) {
    shadowFilter = SDL_CreateTexture(
        GameManager::getRenderer(), SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET, GameManager::getGameWindowSize().x,
        GameManager::getGameWindowSize().y);
    SDL_SetTextureBlendMode(shadowFilter, SDL_BLENDMODE_MOD);
  }

  void resetShadowFilter() {
    // Clear Shadow Filter
    SDL_Rect screenRect = {0, 0, (int)GameManager::getGameWindowSize().x,
                           (int)GameManager::getGameWindowSize().y};
    SDL_SetRenderTarget(GameManager::getRenderer(), shadowFilter);
    SDL_RenderClear(GameManager::getRenderer());
    SDL_SetRenderDrawColor(GameManager::getRenderer(), shadowFilterTint[0],
                           shadowFilterTint[1], shadowFilterTint[2], 255);
    SDL_RenderFillRect(GameManager::getRenderer(), &screenRect);
    SDL_SetRenderTarget(GameManager::getRenderer(), nullptr);
  }

  void update() override {
    SDL_Rect screenRect = {0, 0, (int)GameManager::getGameWindowSize().x,
                           (int)GameManager::getGameWindowSize().y};
    SDL_RenderCopy(GameManager::getRenderer(), shadowFilter, nullptr, &screenRect);
  }

  std::array<Uint8, 3> shadowFilterTint = {255, 255, 255};
  SDL_Texture *shadowFilter;
};
