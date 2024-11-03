#include "ldtk/LDTK_EntityInstance.hpp"
#include "Entity.hpp"
#include "GameManager.hpp"
#include "ldtk/LDTK_EntityData.hpp"
#include "ldtk/LDTK_EntityDefinition.hpp"
#include "ldtk/LDTK_Level.hpp"
#include "ldtk/LDTK_Project.hpp"
#include <stdexcept>

namespace LDTK {
EntityInstance::EntityInstance(const json &entityDefJson, Project *_project) {
  project = _project;

  iid = entityDefJson["iid"];
  box = Box(entityDefJson["px"][0], entityDefJson["px"][1],
            entityDefJson["width"], entityDefJson["height"]);
  defUid = entityDefJson["defUid"];

  entityDefintion = project->entityDefinitions[defUid].get();

  for (const json &fieldInstanceJson : entityDefJson["fieldInstances"]) {
    std::string identifier = fieldInstanceJson["__identifier"];
    FieldInstance field(fieldInstanceJson, project);
    fieldInstances.emplace(identifier, field);
  }
}

Entity *EntityInstance::create(Level *level) {
  entity = GameManager::createEntity(entityDefintion->identifier);
  entity->box.position = box.position + level->levelBox.position;
  entity->box.size = box.size;
  entity->add<EntityData>()->instance = this;
  entity->add<EntityData>()->level = level;
  return entity;
}


Entity* EntityInstance::getEntity() {
  if (entity) {
    return entity;
  }

  throw std::runtime_error("Entity does not exist");
}
} // namespace LDTK
