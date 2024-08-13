#include "ldtk/LDTK_FieldInstance.hpp"
#include "Entity.hpp"
#include "Image.hpp"
#include "ldtk/LDTK_EntityData.hpp"
#include "ldtk/LDTK_Tileset.hpp"
#include <format>
#include <stdexcept>

namespace LDTK {
FieldInstance::FieldInstance(const json &fieldInstanceJson,
                             std::map<int, Tileset *> &_tilesets) {
  tilesets = _tilesets;

  identifier = fieldInstanceJson["__identifier"];
  type = fieldInstanceJson["__type"];
  value = fieldInstanceJson["__value"];
}

bool FieldInstance::isNull() { return value.is_null(); }

std::any FieldInstance::getValue() {
  if (value.is_null()) {
    throw std::runtime_error("Field Instance is null\n");
  }

  if (type == "String" || type.substr(0, 9) == "LocalEnum" || type == "Color" ||
      type == "FilePath") {
    return (std::string)value;
  }

  else if (type == "Int") {
    return (int)value;
  }

  else if (type == "Float") {
    return (float)value;
  }

  else if (type == "Bool") {
    return (bool)value;
  }

  else if (type == "Tile") {
    int x = (int)value["x"] / (int)value["w"];
    int y = (int)value["y"] / (int)value["h"];
    return tilesets[value["tilesetUid"]]->dictionary[TileLoc(x, y)];
  }

  if (type == "Entity_ref") {
    throw std::runtime_error("Field Instance Entity ref not supported\n");
  }

  else if (type == "Point") {
    return Vector2f(value["cx"], value["cy"]);
  }

  throw std::runtime_error("Field Instance not found\n");
}
} // namespace LDTK
