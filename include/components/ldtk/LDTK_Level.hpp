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
struct Project;

struct Level {
  Level(const json &levelJson, Project *_project);
  EntityLayer* getEntitylayerWithIID(std::string iid);

  void load();
  void unload();

  std::vector<Entity *> loadedEntites;

  std::vector<std::unique_ptr<TileLayer>> tileLayers;
  std::vector<std::unique_ptr<EntityLayer>> entityLayers;

  Project *project = nullptr;

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
  std::vector<std::string> neighbours;
};
} // namespace LDTK
