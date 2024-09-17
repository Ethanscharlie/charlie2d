#include "ldtk/LDTK_EntityLayer.hpp"
#include "ldtk/LDTK_EntityDefinition.hpp"
#include "ldtk/LDTK_EntityInstance.hpp"
#include "ldtk/LDTK_LayerDefinition.hpp"
#include "ldtk/LDTK_Project.hpp"
#include <memory>
#include <stdexcept>

namespace LDTK {
EntityLayer::EntityLayer(const json &entityLayerJson, Project *_project) {

  iid = entityLayerJson["iid"];
  levelId = entityLayerJson["levelId"];
  layerDefUid = entityLayerJson["layerDefUid"];
  pxOffsetX = entityLayerJson["pxOffsetX"];
  pxOffsetY = entityLayerJson["pxOffsetY"];
  visible = entityLayerJson["visible"];
  seed = entityLayerJson["seed"];

  project = _project;
  layerDefinition = project->layerDefinitions[layerDefUid].get();

  for (const json &instance : entityLayerJson["entityInstances"]) {
    entityInstances.push_back(
        std::make_unique<EntityInstance>(instance, project));
  }
}

EntityInstance* EntityLayer::getInstanceFromIID(std::string iid) {
  for (auto& entityInstance : entityInstances) {
    if (entityInstance->iid == iid) {
      return entityInstance.get();
    }
  }

  throw std::runtime_error(std::format("Entity instance {} was not found", iid));
}
} // namespace LDTK
