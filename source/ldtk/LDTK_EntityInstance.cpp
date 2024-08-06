#include "ldtk/LDTK_EntityInstance.hpp"
#include "ldtk/LDTK_EntityDefinition.hpp"

namespace LDTK {
EntityInstance::EntityInstance(const json &entityDefJson,
                               EntityDefinition *_entityDefintion) {
  EntityDefinition *entityDefintion = _entityDefintion;

  std::string iid = entityDefJson["iid"];
  Box box = Box(entityDefJson["px"][0], entityDefJson["px"][0],
                entityDefJson["width"], entityDefJson["height"]);
  int defUid = entityDefJson["defUid"];
  // TODO
  //  fieldInstances;
}
} // namespace LDTK
