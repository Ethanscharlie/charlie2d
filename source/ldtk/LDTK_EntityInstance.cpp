#include "ldtk/LDTK_EntityInstance.hpp"
#include "Entity.hpp"
#include "GameManager.hpp"
#include "ldtk/LDTK_EntityData.hpp"
#include "ldtk/LDTK_EntityDefinition.hpp"
#include "ldtk/LDTK_Level.hpp"

namespace LDTK {
EntityInstance::EntityInstance(const json &entityDefJson,
                               EntityDefinition *_entityDefintion,
                               std::map<int, Tileset *> &_tilesets) {
  entityDefintion = _entityDefintion;

  iid = entityDefJson["iid"];
  box = Box(entityDefJson["px"][0], entityDefJson["px"][1],
            entityDefJson["width"], entityDefJson["height"]);
  defUid = entityDefJson["defUid"];

  tilesets = _tilesets;

  for (const json &fieldInstanceJson : entityDefJson["fieldInstances"]) {
    std::string identifier = fieldInstanceJson["__identifier"];
    FieldInstance field(fieldInstanceJson, tilesets);
    fieldInstances.emplace(identifier, field);
  }
}

Entity *EntityInstance::create(Level *level) {
  Entity *entity = GameManager::createEntity(entityDefintion->identifier);
  entity->box.position = box.position + level->levelBox.position;
  entity->box.size = box.size;
  entity->add<EntityData>()->instance = this;
  entity->add<EntityData>()->level = level;
  return entity;
}
} // namespace LDTK
