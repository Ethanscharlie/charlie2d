#pragma once
#include "Box.hpp"
#include "Math.hpp"
#include "ldtk/LDTK_EntityDefinition.hpp"
#include "ldtk/LDTK_FieldInstance.hpp"
#include <string>

namespace LDTK {
struct Level;
struct Project;
struct EntityData;
struct EntityInstance {
  friend struct EntityData;

  EntityInstance(const json &entityDefJson, Project *_project);
  Entity *create(Level *level);
  Entity* getEntity();

  Project *project = nullptr;
  EntityDefinition *entityDefintion;

  std::string iid;
  Box box;
  int defUid;
  std::map<std::string, FieldInstance> fieldInstances;

private:
  Entity *entity = nullptr;
};
} // namespace LDTK
