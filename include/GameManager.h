#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "InputManager.h"
#include "Math.h"
#include "ResourceManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

class Scene;

/**
 * \brief A static class for charlie2d
 *
 * Please use as GameManager::
 */
class GameManager {
public:
  GameManager();
  ~GameManager();

  /**
   * \brief initalizer for charlie2d
   * Must Be Called before anything in charlie2d can also set the original
   * screen resolution
   */
  static void init(Vector2f originalSize = {1920, 1080});
  /**
   * \brief Adds a Scene to the scene map
   * \param name The name given to the scene (used for loading)
   * \param scene the scene to be added
   */
  static void AddScene(const std::string &name, Scene *scene);
  /**
   * \brief Unloads all entities and loads the set scene
   * \param name the scenes name
   */
  static void LoadScene(const std::string &name);
  /**
   * \brief Gets the current scene loaded by LoadScene()
   * \return the current Scene
   */
  static Scene *getCurrentScene();
  /**
   * \brief the main charlie2d update loop method
   */
  static void Update();
  /**
   * \brief plays a soundfile, uses the ResourceManager so there is no need to
   * worry about managing the file loading yourself
   * \param filename the name of the file
   * \param loop yeah you get the idea
   */
  static void playSound(std::string filename, bool loop = false);
  /**
   * \brief Ends and closes everything (runs on sdl2 window close)
   */
  static void quit();
  /**
   * \brief starts the update loop
   *
   * Don't put anything after running this method because it won't run until the
   * game is stopped (meant to be called in your main function after a scene in
   * loaded)
   */
  static void doUpdateLoop();
  /**
   * \brief Changes the windows size
   *
   * This doesn't effect the original size and yes charlie2d supports screen
   * resizing... mostly... some camera bugs need to be fixed \param size a
   * Vector2f for the size, can be set like {1920, 1000} btw
   */
  static void setWindowSize(Vector2f size);
#ifdef __EMSCRIPTEN__

#endif

  /**
   * \brief Performs a screen fade to black and back transition
   * \param middle code to run at the middle or *black* part of the transition
   * \param fadeTime the time it takes to finish the transition
   */
  static void doFade(
      std::function<void()> middle = []() {}, int fadeTime = 300);

  static SDL_Window *window;
  static SDL_Renderer *renderer;
  static SDL_Texture *screenTexture;
  /**
   * \brief the original resolution
   */
  static Vector2f gameWindowSize;
  /**
   * \brief the current resolution
   */
  static Vector2f currentWindowSize;

  /**
   * \brief ratio between original and current screen sizes
   */
  static float screen_change_scale;
  /**
   * \brief Is the game running? Well it should be...
   */
  static bool running;
  /**
   * \brief transition state for fade transitions
   */
  static int transition; // 0 -> Not Going, 1 -> Going, 2 -> Finished

private:
  /**
   * \brief A map for the scenes
   */
  static std::unordered_map<std::string, Scene *> scenes;
  static Scene *currentScene;
  static Scene *loadingScene;

  static Uint32 fadeStartTime;
  static int fade_time;

  static std::function<void()> onMiddleFade;
};

#endif
/**
 * \brief
 * \param
 */
