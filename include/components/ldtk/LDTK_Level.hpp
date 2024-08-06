#pragma once
#include "Box.hpp"
#include "LDTK_FieldInstance.hpp"
#include "ldtk/LDTK_EntityLayer.hpp"
#include "ldtk/LDTK_TileLayer.hpp"
#include "nlohmann/json.hpp"
#include <memory>
#include <string>

using json = nlohmann::json;

namespace LDTK {
struct FieldInstance;

struct Level {
  Level(const json &levelJson,
        std::map<int, LayerDefinition *> &_layerDefinitions,
        std::map<int, EntityDefinition *> &_entityDefinitions,
        std::map<int, Tileset *> &_tilesets);

  std::vector<std::unique_ptr<TileLayer>> tileLayers;
  std::vector<std::unique_ptr<EntityLayer>> entityLayers;

  std::map<int, LayerDefinition *> layerDefinitions;
  std::map<int, EntityDefinition *> entityDefinitions;
  std::map<int, Tileset *> tilesets;

  Box levelBox;

  std::string identifier;
  std::string iid;
  int uid;
  int worldX;
  int worldY;
  int worldDepth;
  int pxWid;
  int pxHei;
  std::string bgColor;
  int useAutoIdentifier;
  int bgPivotX;
  int bgPivotY;
  std::string smartColor;
  std::vector<FieldInstance> fieldInstance;
};
} // namespace LDTK
