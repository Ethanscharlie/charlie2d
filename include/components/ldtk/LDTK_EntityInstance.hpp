#pragma once
#include "Box.hpp"
#include "Math.hpp"
#include "ldtk/LDTK_EntityDefinition.hpp"
#include "ldtk/LDTK_FieldInstance.hpp"
#include <string>

namespace LDTK {
struct EntityInstance {
  EntityInstance (const json& entityDefJson, EntityDefinition* _entityDefintion);

  EntityDefinition* entityDefintion;

  std::string iid;
  Box box;
  int defUid;
  std::vector<FieldInstance> fieldInstances;

};
} // namespace LDTK
