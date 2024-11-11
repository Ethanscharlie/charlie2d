#include "ldtk/LDTK_Level.hpp"
#include "ldtk/LDTK_EntityData.hpp"
#include "ldtk/LDTK_EntityLayer.hpp"
#include "ldtk/LDTK_FieldInstance.hpp"
#include "ldtk/LDTK_Project.hpp"
#include "ldtk/LDTK_TileLayer.hpp"
#include "ldtk/LDTK_Tilemap.hpp"
#include <memory>
#include <stdexcept>

namespace LDTK {
Level::Level(const json &levelJson, Project *_project) {

  project = _project;

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

  for (const json &neighboursJson : levelJson["__neighbours"]) {
    neighbours.push_back(neighboursJson["levelIid"]);
  }

  for (const json &layer : levelJson["layerInstances"]) {
    int layerDefUid = layer["layerDefUid"];
    LayerDefinition *layerDefinition =
        project->layerDefinitions[layerDefUid].get();
    std::string layerType = layerDefinition->type;

    if (layerType == "Tiles" || layerType == "AutoLayer") {
      int tilesetDefUid = layerDefinition->tilesetDefUid;
      if (project->tilesets.find(tilesetDefUid) == project->tilesets.end()) {
        std::cerr << "Error: tilesetDefUid " << tilesetDefUid
                  << " not in tilesets map\n";
        continue;
      }

      Tileset *tileset = project->tilesets[tilesetDefUid].get();
      tileLayers.push_back(std::make_unique<TileLayer>(layer, project));
    }

    else if (layerType == "Entities") {
      entityLayers.push_back(std::make_unique<EntityLayer>(layer, project));
    }
  }

  for (const json &fieldInstanceJson : levelJson["fieldInstances"]) {
    std::string identifier = fieldInstanceJson["__identifier"];
    FieldInstance field(fieldInstanceJson, project);
    fieldInstances.emplace(identifier, field);
  }
}

void Level::load() {
  for (auto &tileLayerUPtr : tileLayers) {
    TileLayer *tileLayer = tileLayerUPtr.get();
    LayerDefinition *layerDefinition = tileLayer->layerDefinition;
    Entity *renderingEntity = layerDefinition->renderingEntity;
    renderingEntity->addComponent<Tilemap>().layer = tileLayer;
    renderingEntity->addComponent<Tilemap>().renderBox = levelBox;
    renderingEntity->setLayer(layerDefinition->renderingLayer);
  }

  for (auto &entityLayerUPtr : entityLayers) {
    EntityLayer *entityLayer = entityLayerUPtr.get();
    LayerDefinition *layerDefinition = entityLayer->layerDefinition;
    for (auto &instance : entityLayer->entityInstances) {
      Entity *entity = instance->create(this);
      entity->setLayer(layerDefinition->renderingLayer);
      loadedEntites.push_back(entity);
    }
  }
}

void Level::unload() {
  for (Entity *entity : loadedEntites) {
    if (!entity)
      continue;
    entity->destroy();
  }
  loadedEntites.clear();

  for (auto &tileLayerUPtr : tileLayers) {
    TileLayer *tileLayer = tileLayerUPtr.get();
    LayerDefinition *layerDefinition = tileLayer->layerDefinition;
    Entity *renderingEntity = layerDefinition->renderingEntity;
    renderingEntity->addComponent<Tilemap>().layer = nullptr;
  }
}

EntityLayer *Level::getEntitylayerWithIID(std::string iid) {
  for (auto &entityLayer : entityLayers) {
    if (entityLayer->iid == iid) {
      return entityLayer.get();
    }
  }

  throw std::runtime_error(std::format("Entity Layer {} was not found", iid));
}

} // namespace LDTK
