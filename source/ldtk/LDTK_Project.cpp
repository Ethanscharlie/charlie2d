#include "ldtk/LDTK_Project.hpp"

#include "ldtk/LDTK_EntityDefinition.hpp"
#include "ldtk/LDTK_LayerDefinition.hpp"
#include "ldtk/LDTK_Level.hpp"
#include "ldtk/LDTK_Tilemap.hpp"
#include "ldtk/LDTK_Tileset.hpp"
#include "ldtk/LDTK_World.hpp"
#include <fstream>
#include <memory>

namespace LDTK {
Project::Project(const std::string &_jsonPath) {
  jsonPath = _jsonPath;

  std::ifstream file(jsonPath);
  const json& jsonData = json::parse(file);

  std::map<int, LayerDefinition *> worldLayerDefinitions;
  std::map<int, EntityDefinition *> worldEntityDefinitions;
  std::map<int, Tileset *> worldTilesets;

  for (const json &layerJson : jsonData["defs"]["layers"]) {
    int uid = layerJson["uid"];
    layerDefinitions[uid] = std::make_unique<LayerDefinition>(layerJson);

    worldLayerDefinitions[uid] = layerDefinitions[uid].get();
  }
  for (const json &entityDefJson : jsonData["defs"]["entities"]) {
    int uid = entityDefJson["uid"];
    entityDefinitions[uid] = std::make_unique<EntityDefinition>(entityDefJson);

    worldEntityDefinitions[uid] = entityDefinitions[uid].get();
  }
  for (const json &tilesetJson : jsonData["defs"]["tilesets"]) {
    if (tilesetJson["identifier"] == "Internal_Icons") continue;
    int uid = tilesetJson["uid"];
    tilesets[uid] = std::make_unique<Tileset>(tilesetJson, jsonPath);

    worldTilesets[uid] = tilesets[uid].get();
  }

  worlds.push_back(
      std::make_unique<World>(jsonData["levels"], worldLayerDefinitions,
                              worldEntityDefinitions, worldTilesets));

  iid = jsonData["iid"];
  jsonVersion = jsonData["jsonVersion"];
  appBuildId = jsonData["appBuildId"];
  nextUid = jsonData["nextUid"];
  identifierStyle = jsonData["identifierStyle"];
  worldLayout = jsonData["worldLayout"];
  worldGridWidth = jsonData["worldGridWidth"];
  worldGridHeight = jsonData["worldGridHeight"];
  defaultLevelWidth = jsonData["defaultLevelWidth"];
  defaultLevelHeight = jsonData["defaultLevelHeight"];
  defaultPivotX = jsonData["defaultPivotX"];
  defaultPivotY = jsonData["defaultPivotY"];
  defaultGridSize = jsonData["defaultGridSize"];
  defaultEntityWidth = jsonData["defaultEntityWidth"];
  defaultEntityHeight = jsonData["defaultEntityHeight"];
  bgColor = jsonData["bgColor"];
  defaultLevelBgColor = jsonData["defaultLevelBgColor"];
  minifyJson = jsonData["minifyJson"];
  externalLevels = jsonData["externalLevels"];
  exportTiled = jsonData["exportTiled"];
  simplifiedExport = jsonData["simplifiedExport"];
  imageExportMode = jsonData["imageExportMode"];
  exportLevelBg = jsonData["exportLevelBg"];
  backupOnSave = jsonData["backupOnSave"];
  backupLimit = jsonData["backupLimit"];
  levelNamePattern = jsonData["levelNamePattern"];
}

void Project::loadLevel(std::string iid) {
  if (currentLevel)
    currentLevel->unload();

  currentLevel = worlds[0]->levels[iid].get();
  currentLevel->load();

  onLoadLevel();
}
} // namespace LDTK
