#pragma once

#include "Component.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "Vector2f.h"
#include <SDL.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <typeindex>

class Box;
class Entity;

enum class EntityRenderPositionType { World, Screen };

/** @file */

struct Image {
  Image() {
    texture = nullptr;
    path = "";
  }
  Image(std::string _path);
  Image(SDL_Texture *_texture) : path(""), texture(_texture) {}
  SDL_Texture *texture;
  std::string path;
};

/**
 * \brief Random floating point number
 */
float randFloat(float min, float max);

/**
 * \brief Grabs a random element from an array
 */
template <typename T, std::size_t N> T getRandomElement(const T (&arr)[N]);

/**
 * \brief Converts a float to a string without unnessessary zeros
 */
std::string floatToString(float value);

/**
 * \brief Gets a screen position as a logical position
 */
Vector2f getLogcialPosition(Vector2f screenPos);
/**
 * \brief Gets a logical position as a screen position
 */
Vector2f getScreenPosition(Vector2f logicalPos);
/**
 * \brief Should be used for all imgui frames, to use please render imgui
 * without logical rendering first and then turn it back on
 */
Vector2f getImGuiPosition(Vector2f pos);

/**
 * \brief Gets sdl2's logical rect because sdl doesn't offer a way too (Ur
 * Welcome)
 */

SDL_Rect getLogicalRect();

Vector2f getWindowPosition(Vector2f gamePosition);

Vector2f getFullLogicalToWindowPosition(Vector2f fullLogicalPosition);

std::string getTypeNameWithoutNumbers(std::type_index typeIndex);

Box getRenderBox(Box box, EntityRenderPositionType renderPositionType =
                              EntityRenderPositionType::World);
Box getRenderBox(Entity *entity);

std::array<Uint8, 3> hexToRGB(int hexColor);

//
//
//
//
//
//
//

template <typename T, std::size_t N> T getRandomElement(const T (&arr)[N]) {
  // Generate a random index within the array bounds
  std::size_t randomIndex = std::rand() % N;

  // Return the randomly selected element
  return arr[randomIndex];
}
