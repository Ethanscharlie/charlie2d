#pragma once

#include <SDL.h>
#include <SDL_gamecontroller.h>
#include <unordered_map>
#include <vector>
// #include "GameManager.h"
#include "Math.h"
#include "SDL_events.h"

// #define NUM_KEYS 256 // Total number of keys
#define NUM_KEYS 286 // Total number of keys

/**
 * \brief Manages input through sdl2 MY WAY
 */
class InputManager {
public:
  InputManager();
  /**
   * \brief Update loop
   */
  static void update();
  /**
   * \brief Checks for verticle input W / S / UParrow / DOWNarrow
   * \return -1 (Down) 1 (Up) 0 (None)
   */
  static int checkVertical();
  /**
   * \brief Checks for Horizontal input D / A / RIGHTarrow / LEFTarrow
   * \return -1 (Left) 1 (Right) 0 (None)
   */
  static int checkHorizontal();
  /**
   * \brief Check for axis input (read checkVertical and checkHorizontal)
   * \return a Vector2f with each axis as -1, 0 or 1
   */
  static Vector2f checkAxis();

  static bool checkKeyHeld(SDL_Scancode scancode);
  /**
   * \brief Gets the Mouse pointer in world position
   */
  static Vector2f getMouseWorldPosition();
  /**
   * \brief Gets the Mouse pointer in UI position
   */
  static Vector2f getMouseUIPosition();
  /**
   * \brief Gets the mouse raw position
   */
  static Vector2f getMouseScreenPosition();

  static bool mouseHeld;
  static bool leftMouseHeld;
  static bool rightMouseHeld;

  static float mouseScroll;

  /**
   * \brief Made for check events (Not holding down)
   */
  static bool keyEvents[NUM_KEYS];

  static void onEvent(SDL_Event event);
};
