#pragma once
#include "ldtk/LDTK_EntityDefinition.hpp"
#include "ldtk/LDTK_LayerDefinition.hpp"
#include "nlohmann/json.hpp"
#include "ldtk/LDTK_EntityInstance.hpp"
#include <memory>
#include <string>

using json = nlohmann::json;

namespace LDTK {

struct EntityLayer {
  EntityLayer(const json &entityLayerJson, LayerDefinition *_layerDefinition,
              std::map<int, EntityDefinition *> &_entityDefinitions);

  std::string iid;
  int levelId;
  int layerDefUid;
  int pxOffsetX;
  int pxOffsetY;
  int visible;
  int seed;

  std::vector<std::unique_ptr<EntityInstance>> entityInstances;

  const LayerDefinition *layerDefinition;
  std::map<int, EntityDefinition *> entityDefinitions;
};
} // namespace LDTK
