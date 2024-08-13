#pragma once
#include "Entity.hpp"
#include "ldtk/LDTK_Tileset.hpp"
#include "nlohmann/json.hpp"
#include <any>
#include <format>
#include <string>

using json = nlohmann::json;

namespace LDTK {
struct FieldInstance {
  FieldInstance(const json &fieldInstanceJson,
                std::map<int, Tileset *> &_tilesets);
  bool isNull();
  std::any getValue();

  std::map<int, Tileset *> tilesets;

  std::string identifier;
  std::string type;

private:
  json value;
};
} // namespace LDTK
