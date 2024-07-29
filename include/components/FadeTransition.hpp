#pragma once
#include "Component.hpp"
#include "Entity.hpp"
#include "GameManager.hpp"
#include "SDL_stdinc.h"
#include "SDL_timer.h"
#include <functional>

enum Section { FadingOut, FadingIn, Finished };

class FadeTransition : public Component {
public:
  void start() override;
  void update(float deltaTime) override;
  /**
   * \brief Performs a screen fade to black and back transition
   * \param middle code to run at the middle or *black* part of the transition
   * \param fadeTime the time it takes to finish the transition
   */
  void doFade(
      std::function<void()> middle = []() {}, int fadeTime = 300,
      std::function<void()> post = []() {});

  Section currentSection = Finished;
  Uint32 fadeStartTime;
  int fade_time;
  std::function<void()> onMiddleFade;
  std::function<void()> onPostFade;
};
