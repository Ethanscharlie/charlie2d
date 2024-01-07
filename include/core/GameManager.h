#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Component.h"
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
#include <typeindex>
#include <unordered_map>
#include <vector>

// IMGUI
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

class Entity;

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
   * \brief Unloads all entities and loads the set scene
   * \param name the scenes name
   */
  static void Load(const std::string &name);
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
   * \breif Destroys all entities currently alive
   */
  static void destroyAll(Entity doNotDestroy[]);

  /**
   * \brief Creates and Entity
   * \param tag Currently the only way to set an Entitys tag
   * \return The newly made Entity
   */
  static Entity *createEntity(std::string tag = "");

  /**
   * \brief A Vector of very entity in the scene
   */
  static std::vector<Entity *> getAllObjects();

  /**
   * \brief Gets Components via their type
   * \param tag Will only grab Components with entities of a certain tag,
   * Defaults to "" which ignores this Ex `GameManager::getComponents<Sprite>()`
   * Will get every sprite component
   */
  template <typename C>
  static std::vector<C *> getComponents(std::string tag = "");

  /**
   * \brief Removes a component
   */
  static void removeComponent(Component *component, std::type_index type);

  /**
   * \brief Gets entitys via their tags
   * \param tag string for the tag
   * \return An std::vector of entitys
   */
  static std::vector<Entity *> getEntities(std::string tag);

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

  /**
   * \brief Performs a screen fade to black and back transition
   * \param middle code to run at the middle or *black* part of the transition
   * \param fadeTime the time it takes to finish the transition
   */
  static void doFade(
      std::function<void()> middle = []() {}, int fadeTime = 300, std::function<void()> post = []() {});

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

  /**
   * \brief Pause mode
   */
  static bool updateEntities;
  static float deltaTime;

  static std::map<std::type_index, std::vector<Component *>> components;
  static std::map<std::string, std::vector<Entity *>>
      entities; // Sorted by tags

private:
  /**
   * Deletes
   */
  static void destroyEntity(Entity* entity);

  static Uint32 fadeStartTime;
  static int fade_time;

  static Uint64 lastTime;

  static std::function<void()> onMiddleFade;
  static std::function<void()> onPostFade;
};

template <typename C>
std::vector<C *> GameManager::getComponents(std::string tag) {
  std::vector<C *> hits;
  auto it = components.find(typeid(C));

  if (it != components.end()) {
    for (auto component : it->second) {
      // Static cast each Component* to C*
      if (component->entityTag == tag || tag == "") {
        hits.push_back(static_cast<C *>(component));
      }
    }
  } else
    components[typeid(C)];

  return hits;
}

#endif
/**
 * \brief
 * \param
 */
