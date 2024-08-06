#pragma once
#include "Math.hpp"
#include "ldtk/LDTK_EntityDefinition.hpp"
#include "ldtk/LDTK_LayerDefinition.hpp"
#include "ldtk/LDTK_Tileset.hpp"
#include "nlohmann/json.hpp"
#include <memory>

using json = nlohmann::json;

namespace LDTK {
struct Level;

struct World {
  World(const json &levelsJson,
        std::map<int, LayerDefinition *> &_layerDefinitions,
        std::map<int, EntityDefinition *> &_entityDefinitions,
        std::map<int, Tileset *> &_tilesets);

  std::map<int, LayerDefinition *> layerDefinitions;
  std::map<int, EntityDefinition *> entityDefinitions;
  std::map<int, Tileset *> tilesets;

  std::map<std::string, std::unique_ptr<Level>> levels;
};
} // namespace LDTK
