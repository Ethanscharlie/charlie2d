#include "ldtk/LDTK_EntityLayer.hpp"
#include "ldtk/LDTK_EntityDefinition.hpp"
#include "ldtk/LDTK_EntityInstance.hpp"
#include "ldtk/LDTK_LayerDefinition.hpp"
#include <memory>

namespace LDTK {
EntityLayer::EntityLayer(
    const json &entityLayerJson, LayerDefinition *_layerDefinition,
    std::map<int, EntityDefinition *> &_entityDefinitions, std::map<int, Tileset *> &_tilesets) {

  layerDefinition = _layerDefinition;
  entityDefinitions = _entityDefinitions;
  tilesets = _tilesets;

  for (const json &instance : entityLayerJson["entityInstances"]) {
    entityInstances.push_back(std::make_unique<EntityInstance>(
        instance, entityDefinitions[instance["defUid"]], tilesets));
  }

  iid = entityLayerJson["iid"];
  levelId = entityLayerJson["levelId"];
  layerDefUid = entityLayerJson["layerDefUid"];
  pxOffsetX = entityLayerJson["pxOffsetX"];
  pxOffsetY = entityLayerJson["pxOffsetY"];
  visible = entityLayerJson["visible"];
  seed = entityLayerJson["seed"];
}
} // namespace LDTK
