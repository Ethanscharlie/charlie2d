#include "ldtk/LDTK_Project.hpp"

#include "Event.hpp"
#include "ldtk/LDTK_EntityDefinition.hpp"
#include "ldtk/LDTK_LayerDefinition.hpp"
#include "ldtk/LDTK_Level.hpp"
#include "ldtk/LDTK_Tilemap.hpp"
#include "ldtk/LDTK_Tileset.hpp"
#include "ldtk/LDTK_World.hpp"
#include <fstream>
#include <memory>
#include <stdexcept>

namespace LDTK {
Project::Project(const std::string &_jsonPath) {
  jsonPath = _jsonPath;

  std::ifstream file(jsonPath);
  const json &jsonData = json::parse(file);

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

  int renderingLayer = jsonData["defs"]["layers"].size();
  for (const json &layerJson : jsonData["defs"]["layers"]) {
    int uid = layerJson["uid"];
    layerDefinitions[uid] =
        std::make_unique<LayerDefinition>(layerJson, renderingLayer);
    renderingLayer--;
  }
  for (const json &entityDefJson : jsonData["defs"]["entities"]) {
    int uid = entityDefJson["uid"];
    entityDefinitions[uid] = std::make_unique<EntityDefinition>(entityDefJson);
  }
  for (const json &tilesetJson : jsonData["defs"]["tilesets"]) {
    if (tilesetJson["identifier"] == "Internal_Icons")
      continue;
    int uid = tilesetJson["uid"];
    tilesets[uid] = std::make_unique<Tileset>(tilesetJson, jsonPath);
  }

  worlds.push_back(std::make_unique<World>(jsonData["levels"], this));

  Event::addEventListener("screenResize", [this]() { render(); });
}

void Project::loadLevel(std::string iid) {
  if (currentLevel)
    currentLevel->unload();

  currentLevel = worlds[0]->getLevelWithIID(iid);
  currentLevel->load();

  onLoadLevel();
}

void Project::render() {
  std::cout << "Rendering everthing\n";
  for (auto &world : worlds) {
    for (auto &[uid, level] : world->levels) {
      for (auto &tileLayer : level->tileLayers) {
        tileLayer->render(level.get());
      }
    }
  }
}

Level *Project::findTraveledLevel(Box &box) {
  for (std::string levelIid : currentLevel->neighbours) {
    Level *level = worlds[0]->getLevelWithIID(levelIid);
    if (box.checkCollision(level->levelBox)) {
      return level;
    }
  }

  throw std::runtime_error("No traveled level\n");
}
} // namespace LDTK
