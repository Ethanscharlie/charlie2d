#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Component.hpp"
#include "Entity.hpp"
#include "InputManager.hpp"
#include "Math.hpp"
#include "SDL2_framerate.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <array>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

// IMGUI
#include "SDL_render.h"
#include "SDL_surface.h"
#include "Vector2f.hpp"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

template <typename T> class ComponentData;
class ShadowFilter;

namespace GameManager {
/**
 * \brief initalizer for charlie2d
 * Must Be Called before anything in charlie2d can also set the original
 * screen resolution
 */
void init(Vector2f originalSize = {1920, 1080});
/**
 * \brief the main charlie2d update loop method
 */
void Update();
/**
 * \brief Ends and closes everything (runs on sdl2 window close)
 */
void quit();

/**
 * \breif Destroys all entities currently alive
 */
void destroyAll();

/**
 * \brief Creates and Entity
 * \param tag Currently the only way to set an Entitys tag
 * \return The newly made Entity
 */
Entity &createEntity(std::string tag);

/**
 * \brief Gets Components via their type
 * \param tag Will only grab Components with entities of a certain tag,
 * Defaults to "" which ignores this Ex `GameManager::getComponents<Sprite>()`
 * Will get every sprite component
 */
template <typename C> std::vector<C *> getComponents(std::string tag = "");

/**
 * \brief Removes a component
 */
void removeComponent(Component *component, std::type_index type);

/**
 * \brief Gets entitys via their tags
 * \param tag string for the tag
 * \return An std::vector of entitys
 */
std::vector<Entity *> getEntities(std::string tag);

SDL_Renderer *getRenderer();
SDL_Window *getWindow();

/**
 * \brief A Vector of very entity in the scene
 */
std::vector<Entity *> getEntities();

/**
 * \brief starts the update loop
 *
 * Don't put anything after running this method because it won't run until the
 * game is stopped (meant to be called in your main function after a scene in
 * loaded)
 */
void doUpdateLoop();
/**
 * \brief Changes the windows size
 *
 * This doesn't effect the original size and yes charlie2d supports screen
 * resizing... mostly... some camera bugs need to be fixed \param size a
 * Vector2f for the size, can be set like {1920, 1000} btw
 */
void setWindowSize(Vector2f size);

/**
 * \brief the original resolution used for calculations
 */
Vector2f getGameWindowSize();

/**
 * \brief the current resolution, only use if you know what your doing
 */
Vector2f getCurrentWindowSize();

/**
 * \breif Adds a component to the components map (Used by Entity class)
 */
template <typename C> void addComponent(C *component);

/**
 * \breif Time in between frames, you should multiply this to any frame based
 *  iteration, like player movement static float deltaTime;
 */
float getDeltaTime();

/**
 * \brief is called when an entity is added or a layer is changed.
 * Resorts the entire entity list based on layers
 */
void resortEntitiesNextFrame();

} // namespace GameManager

template <typename C>
std::vector<C *> GameManager::getComponents(std::string tag) {
  std::vector<C *> components;
  for (Entity *entity : getEntities()) {
    if (entity->checkComponent<C>()) {
      components.push_back(&entity->getComponent<C>());
    }
  }
  return components;
}

#endif
/**
 * \brief
 * \param
 */
