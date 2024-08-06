#pragma once

#include "LDTKEntity.hpp"
#include <string>
#include <vector>
#include "Math.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace LDTK {
struct EntityDefinition {
  EntityDefinition(const json &entityDefJson);
  
  std::string identifier;
  int uid;
  std::vector<std::string> tags;
  bool allowOutOfBounds;
  int width;
  int height;
  int tileOpacity;
  float fillOpacity;
  int lineOpacity;
  bool hollow;
  std::string color;
  std::string renderMode;
  int tilesetId;
  std::string tileRenderMode;
  // std::vector<int> nineSliceBorders;
  int pivotX;
  int pivotY;
};
} // namespace LDTK
