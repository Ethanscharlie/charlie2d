#ifndef ANIMATION_H
#define ANIMATION_H

#include "Image.h"
#include "ResourceManager.h"
#include "SDL_stdinc.h"
#include "SDL_timer.h"
#include <functional>
#include <string>

class Entity;
class Sprite;

/**
 * \brief ANIMATIONS YEAH!!!
 * Meant to be used the the Sprite Component
 */
class Animation {
public:
  /**
   * \brief Uhh yeah
   * \param spriteIn the Sprite Component using the animation
   * \param framesIn the filenames for the frames in the animation
   * \param fpsIn fps stands for faps-Uhh I mean frames per second
   */
  Animation(Sprite *spriteIn, std::vector<Image> framesIn, float fpsIn);
  /**
   * \brief update loop for the animation
   *
   * Don't worry it still uses the timer inside it and doesn't just switch
   * frames
   */
  void update();
  /**
   * \brief resets the frame counter
   *
   * TODO remember what this method does because I forgor tbh
   */
  void resetFrameCounter();
  /**
   * \brief Starts the animation
   * \param direction can be 1 or -1 (to play backwards)
   * \param onFinishIn code to run on animation finish
   */
  void play(
      int direction = 1, std::function<void()> onFinishIn = []() {});
  /**
   * \brief Tells the animation to loop and not just play
   * \param direction can be 1 or -1 (to play backwards)
   */
  void loop(int direction = 1);

  /**
   * \brief The Sprite Component for the rendering
   */
  Sprite *sprite;
  std::vector<Image> frames;
  Uint32 timer = SDL_GetTicks();
  float fps = 1;
  int frameCounter = 0;
  int direction = 1;
  bool playing{};
  bool looping{};
  std::function<void()> onFinish;
};

#endif
