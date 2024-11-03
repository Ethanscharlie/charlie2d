#pragma once
#include "Entity.hpp"
#include "ldtk/LDTK_Tileset.hpp"
#include "nlohmann/json.hpp"
#include <any>
#include <format>
#include <string>

using json = nlohmann::json;

namespace LDTK {
struct Project;
struct FieldInstance {
  FieldInstance(const json &fieldInstanceJson,
                Project* _project);
  bool isNull();
  std::any getValue();

  Project* project;

  std::string identifier;
  std::string type;

private:
  json value;
};
} // namespace LDTK
