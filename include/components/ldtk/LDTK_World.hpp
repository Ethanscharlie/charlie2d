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
struct Project;

struct World {
  World(const json &levelsJson, Project* _project);
  Level* getLevelWithIID(std::string iid);

  Project* project;

  std::map<int, std::unique_ptr<Level>> levels;
};
} // namespace LDTK
