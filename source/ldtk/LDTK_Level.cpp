#include "ldtk/LDTK_Level.hpp"
#include "ldtk/LDTK_EntityLayer.hpp"
#include "ldtk/LDTK_FieldInstance.hpp"
#include "ldtk/LDTK_TileLayer.hpp"
#include "ldtk/LDTK_Tilemap.hpp"
#include <memory>

namespace LDTK {
Level::Level(const json &levelJson,
             std::map<int, LayerDefinition *> &_layerDefinitions,
             std::map<int, EntityDefinition *> &_entityDefinitions,
             std::map<int, Tileset *> &_tilesets) {

  layerDefinitions = _layerDefinitions;
  entityDefinitions = _entityDefinitions;
  tilesets = _tilesets;

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

  levelBox = Box(worldX, worldY, pxWid, pxHei);

  for (const json &layer : levelJson["layerInstances"]) {
    int layerDefUid = layer["layerDefUid"];
    LayerDefinition *layerDefinition = layerDefinitions[layerDefUid];
    std::string layerType = layerDefinition->type;

    if (layerType == "Tiles" || layerType == "AutoLayer") {
      int tilesetDefUid = layerDefinition->tilesetDefUid;
      if (tilesets.find(tilesetDefUid) == tilesets.end()) {
        std::cerr << "Error: tilesetDefUid " << tilesetDefUid
                  << " not in tilesets map\n";
        continue;
      }

      Tileset *tileset = tilesets[tilesetDefUid];
      tileLayers.push_back(
          std::make_unique<TileLayer>(layer, layerDefinition, tileset, this));
    }

    else if (layerType == "Entities") {
      entityLayers.push_back(std::make_unique<EntityLayer>(
          layer, layerDefinition, entityDefinitions, tilesets));
    }
  }

  std::vector<std::unique_ptr<TileLayer>> tileLayers;
  std::vector<std::unique_ptr<EntityLayer>> entityLayers;

  // TODO
  // fieldInstance = levelJson["fieldInstances"];
}

void Level::load() {
  for (auto &tileLayerUPtr : tileLayers) {
    TileLayer *tileLayer = tileLayerUPtr.get();
    LayerDefinition *layerDefinition = tileLayer->layerDefinition;
    Entity *renderingEntity = layerDefinition->renderingEntity;
    renderingEntity->add<Tilemap>()->layer = tileLayer;
    levelBox.print();
    renderingEntity->add<Tilemap>()->renderBox = levelBox;
  }

  for (auto &entityLayerUPtr : entityLayers) {
    EntityLayer *entityLayer = entityLayerUPtr.get();
    LayerDefinition *layerDefinition = entityLayer->layerDefinition;
    for (auto &instance : entityLayer->entityInstances) {
      Entity *entity = instance->create(this);
      loadedEntites.push_back(entity);
    }
  }
}

void Level::unload() {
  for (Entity *entity : loadedEntites) {
    entity->toDestroy = true;
  }
  loadedEntites.clear();

  for (auto &tileLayerUPtr : tileLayers) {
    TileLayer *tileLayer = tileLayerUPtr.get();
    LayerDefinition *layerDefinition = tileLayer->layerDefinition;
    Entity *renderingEntity = layerDefinition->renderingEntity;
    renderingEntity->add<Tilemap>()->layer = nullptr;
  }
}

} // namespace LDTK
