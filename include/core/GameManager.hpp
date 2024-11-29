#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Component.hpp"
#include "InputManager.hpp"
#include "Math.hpp"
#include "ResourceManager.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <map>
#include <SDL_ttf.h>
#include <array>
#include <functional>
#include <iostream>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include "SDL2_framerate.h"

// IMGUI
#include "SDL_render.h"
#include "SDL_surface.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

template <typename T> class ComponentData;
class Entity;
class ShadowFilter;

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
   * \brief the main charlie2d update loop method
   */
  static void Update();
  /**
   * \brief Ends and closes everything (runs on sdl2 window close)
   */
  static void quit();

  /**
   * \breif Destroys all entities currently alive
   */
  static void destroyAll();

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

  static void changeEntityTag(Entity *entity, std::string newTag);

  /**
   * \breif Adds a component to the components map (Used by Entity class)
   */
  template <typename C> static void addComponent(C *component);

  static SDL_Window *window;
  static SDL_Renderer *renderer;

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
   * \brief Pause mode
   */
  static bool updateEntities;

  static float deltaTime;

  /**
   * \breif Time in between frames, you should multiply this to any frame base
iteration, like player movement static float deltaTime;
  */

  static SDL_Texture *targetRenderSurface;

  static std::map<std::string, std::function<Component *(Entity *)>>
      componentRegistry;

  static std::map<std::string, std::vector<Entity *>> entities;

  static ShadowFilter *shadowFilter;

private:
  /**
   * Deletes
   */
  static void destroyEntity(Entity *entity);

  static std::map<std::type_index, std::vector<Component *>> components;

  static Uint64 lastTime;

  static FPSmanager fpsManager;
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

inline std::map<std::string, std::function<Component *(Entity *)>>
    GameManager::componentRegistry;

template <typename C> void GameManager::addComponent(C *component) {
  components[typeid(C)].push_back(component);
}

#endif
/**
 * \brief
 * \param
 */
