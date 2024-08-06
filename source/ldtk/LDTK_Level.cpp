#include "ldtk/LDTK_Level.hpp"
#include "ldtk/LDTK_EntityLayer.hpp"
#include "ldtk/LDTK_FieldInstance.hpp"
#include "ldtk/LDTK_TileLayer.hpp"
#include <memory>

namespace LDTK {
Level::Level(const json &levelJson,
             std::map<int, LayerDefinition *> &_layerDefinitions,
             std::map<int, EntityDefinition *> &_entityDefinitions,
             std::map<int, Tileset *> &_tilesets) {

  layerDefinitions = _layerDefinitions;
  entityDefinitions = _entityDefinitions;
  tilesets = _tilesets;

  for (const json &layer : levelJson["layerInstances"]) {
    int layerDefUid = layer["layerDefUid"];
    LayerDefinition *layerDefinition = layerDefinitions[layerDefUid];
    std::string layerType = layerDefinition->type;

    int tilesetDefUid = layerDefinition->tilesetDefUid;
    Tileset *tileset = tilesets[tilesetDefUid];

    if (layerType == "Tiles" || layerType == "AutoLayer") {
      tileLayers.push_back(std::make_unique<TileLayer>(
          TileLayer(layer, layerDefinition, tileset)));
    }

    else if (layerType == "Entities") {
      entityLayers.push_back(std::make_unique<EntityLayer>(
          EntityLayer(layer, layerDefinition, entityDefinitions)));
    }
  }

  std::vector<std::unique_ptr<TileLayer>> tileLayers;
  std::vector<std::unique_ptr<EntityLayer>> entityLayers;

  identifier = levelJson["identifier"];
  iid = levelJson["iid"];
  uid = levelJson["uid"];
  worldX = levelJson["worldX"];
  worldY = levelJson["worldY"];
  worldDepth = levelJson["worldDepth"];
  pxWid = levelJson["pxWid"];
  pxHei = levelJson["pxHei"];
  bgColor = levelJson["__bgColor"];
  useAutoIdentifier = levelJson["useAutoIdentifier"];
  bgPivotX = levelJson["bgPivotX"];
  bgPivotY = levelJson["bgPivotY"];
  smartColor = levelJson["__smartColor"];
  // TODO
  // fieldInstance = levelJson["fieldInstances"];

  levelBox = Box(worldX, worldY, pxWid, pxHei);
}
} // namespace LDTK
