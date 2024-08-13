#pragma once
#include "Box.hpp"
#include "Math.hpp"
#include "ldtk/LDTK_EntityDefinition.hpp"
#include "ldtk/LDTK_FieldInstance.hpp"
#include <string>

namespace LDTK {
struct Level;
struct EntityInstance {
  EntityInstance(const json &entityDefJson, EntityDefinition *_entityDefintion, std::map<int, Tileset *> &_tilesets);
  Entity *create(Level *level);

  EntityDefinition *entityDefintion;
  std::map<int, Tileset *> tilesets;

  std::string iid;
  Box box;
  int defUid;
  std::map<std::string, FieldInstance> fieldInstances;
};
} // namespace LDTK
