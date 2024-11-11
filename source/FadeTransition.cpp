#include "FadeTransition.hpp"

FadeTransition::FadeTransition(Entity& entity) : Component(entity) {
  entity.setLayer(120);
  typeIsRendering = true;
}

void FadeTransition::update() {
  SDL_Rect fill_dst;
  Uint32 elapsedTime = 0;

  // Used for FadingOut and FadingIn so why repeat the code
  if (currentSection != Finished) {
    fill_dst.w = GameManager::getGameWindowSize().x;
    fill_dst.h = GameManager::getGameWindowSize().y;
    fill_dst.x = 0;
    fill_dst.y = 0;

    elapsedTime = SDL_GetTicks() - fadeStartTime;
  }

  switch (currentSection) {
  case Finished:
    break;

  case FadingOut:
    SDL_SetRenderDrawBlendMode(GameManager::getRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(GameManager::getRenderer(), 0, 0, 0,
                           static_cast<Uint8>(255 * elapsedTime / fade_time));

    SDL_RenderFillRect(GameManager::getRenderer(), &fill_dst);

    if (elapsedTime >= fade_time) {
      currentSection = FadingIn;
      fadeStartTime = SDL_GetTicks();

      onMiddleFade();
    }
    break;

  case FadingIn:
    SDL_SetRenderDrawBlendMode(GameManager::getRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(
        GameManager::getRenderer(), 0, 0, 0,
        static_cast<Uint8>(255 - 255 * elapsedTime / fade_time));

    SDL_RenderFillRect(GameManager::getRenderer(), &fill_dst);

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
void FadeTransition::doFade(std::function<void()> middle, int fadeTime,
                            std::function<void()> post) {
  fade_time = fadeTime;
  onMiddleFade = middle;
  onPostFade = post;
  currentSection = FadingOut;
  fadeStartTime = SDL_GetTicks();
}
