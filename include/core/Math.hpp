#pragma once

#include "Component.hpp"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "Vector2f.hpp"
#include "nlohmann/json.hpp"
#include <SDL.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <typeindex>

class Box;
class Entity;
struct Image;

enum class EntityRenderPositionType { World, Screen };

/** @file */

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

std::vector<Image> generateSpritesheetAnimation(Image image, int gridSize = 16,
                                                int row = 0);

template <typename T> std::vector<T> jsonToVector(const nlohmann::json &j) {
  std::vector<T> result;

  if (j.is_array()) {
    for (const auto &item : j) {
      if (item.is_number_integer()) {
        result.push_back(item.get<int>());
      } else if (item.is_number_float()) {
        result.push_back(item.get<float>());
      } else if (item.is_string()) {
        result.push_back(item.get<std::string>());
      } else if (item.is_boolean()) {
        result.push_back(item.get<bool>());
      }
    }
  }

  return result;
}

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
