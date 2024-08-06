#pragma once
#include "ldtk/LDTK_Level.hpp"
#include "ldtk/LDTK_World.hpp"
#include "nlohmann/json.hpp"
#include <memory>
#include <string>

using json = nlohmann::json;

namespace LDTK {
struct LayerDefinition;
struct EntityDefinition;
struct Tileset;

struct Project {
  Project(const json &jsonData, const std::string& _jsonPath);
  void loadLevel(std::string iid);

  Level* currentLevel = nullptr;

  std::map<int, std::unique_ptr<LayerDefinition>> layerDefinitions;
  std::map<int, std::unique_ptr<EntityDefinition>> entityDefinitions;
  std::map<int, std::unique_ptr<Tileset>> tilesets;

  std::vector<std::unique_ptr<World>> worlds;

  std::string jsonPath;

  std::string iid;
  std::string jsonVersion;
  int appBuildId;
  int nextUid;
  std::string identifierStyle;
  std::string worldLayout;
  int worldGridWidth;
  int worldGridHeight;
  int defaultLevelWidth;
  int defaultLevelHeight;
  int defaultPivotX;
  int defaultPivotY;
  int defaultGridSize;
  int defaultEntityWidth;
  int defaultEntityHeight;
  std::string bgColor;
  std::string defaultLevelBgColor;
  bool minifyJson;
  bool externalLevels;
  bool exportTiled;
  bool simplifiedExport;
  std::string imageExportMode;
  bool exportLevelBg;
  bool backupOnSave;
  int backupLimit;
  std::string levelNamePattern;
};
} // namespace LDTK
