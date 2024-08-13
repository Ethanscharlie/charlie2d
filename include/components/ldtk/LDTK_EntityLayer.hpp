#pragma once
#include "ldtk/LDTK_EntityDefinition.hpp"
#include "ldtk/LDTK_EntityInstance.hpp"
#include "ldtk/LDTK_LayerDefinition.hpp"
#include "ldtk/LDTK_Tileset.hpp"
#include "nlohmann/json.hpp"
#include <memory>
#include <string>

using json = nlohmann::json;

namespace LDTK {
struct Project;

struct EntityLayer {
  EntityLayer(const json &entityLayerJson, Project *_project);
  EntityInstance* getInstanceFromIID(std::string iid);

  std::string iid;
  int levelId;
  int layerDefUid;
  int pxOffsetX;
  int pxOffsetY;
  int visible;
  int seed;

  std::vector<std::unique_ptr<EntityInstance>> entityInstances;

  Project* project;
  LayerDefinition *layerDefinition;
};
} // namespace LDTK
