#pragma once
#include "Component.h"
#include "Entity.h"
#include "GameManager.h"
#include "SDL_stdinc.h"
#include "SDL_timer.h"
#include <functional>

enum Section { FadingOut, FadingIn, Finished };

class FadeTransition : public Component {
public:
  void start() override {
    entity->useLayer = true;
    entity->layer = 120;
    typeIsRendering = true;
  }

  void update(float deltaTime) override {
    SDL_Rect fill_dst;
    Uint32 elapsedTime;

    // Used for FadingOut and FadingIn so why repeat the code
    if (currentSection != Finished) {
      fill_dst.w = GameManager::gameWindowSize.x;
      fill_dst.h = GameManager::gameWindowSize.y;
      fill_dst.x = 0;
      fill_dst.y = 0;

      elapsedTime = SDL_GetTicks() - fadeStartTime;
    }

    switch (currentSection) {
    case Finished:
      break;

    case FadingOut:
      SDL_SetRenderDrawBlendMode(GameManager::renderer, SDL_BLENDMODE_BLEND);
      SDL_SetRenderDrawColor(GameManager::renderer, 0, 0, 0,
                             static_cast<Uint8>(255 * elapsedTime / fade_time));

      SDL_RenderFillRect(GameManager::renderer, &fill_dst);

      if (elapsedTime >= fade_time) {
        currentSection = FadingIn;
        fadeStartTime = SDL_GetTicks();

        onMiddleFade();
      }
      break;

    case FadingIn:
      SDL_SetRenderDrawBlendMode(GameManager::renderer, SDL_BLENDMODE_BLEND);
      SDL_SetRenderDrawColor(
          GameManager::renderer, 0, 0, 0,
          static_cast<Uint8>(255 - 255 * elapsedTime / fade_time));

      SDL_RenderFillRect(GameManager::renderer, &fill_dst);

      if (elapsedTime >= fade_time) {
        currentSection = Finished;
        onPostFade();
      }
      break;
    }
  }

  /**
   * \brief Performs a screen fade to black and back transition
   * \param middle code to run at the middle or *black* part of the transition
   * \param fadeTime the time it takes to finish the transition
   */
  void doFade(
      std::function<void()> middle = []() {}, int fadeTime = 300,
      std::function<void()> post = []() {}) {
    fade_time = fadeTime;
    onMiddleFade = middle;
    onPostFade = post;
    currentSection = FadingOut;
    fadeStartTime = SDL_GetTicks();
  }

  Section currentSection = Finished;
  Uint32 fadeStartTime;
  int fade_time;
  std::function<void()> onMiddleFade;
  std::function<void()> onPostFade;
};
