#include "ldtk/LDTK_EntityDefinition.hpp"

namespace LDTK {
EntityDefinition::EntityDefinition(const json &entityDefJson) {
  identifier = entityDefJson["identifier"];
  uid = entityDefJson["uid"];
  for (const json& tag : entityDefJson["tags"]) {
    tags.push_back(tag);
  }
  allowOutOfBounds = entityDefJson["allowOutOfBounds"];
  width = entityDefJson["width"];
  height = entityDefJson["height"];
  tileOpacity = entityDefJson["tileOpacity"];
  fillOpacity = entityDefJson["fillOpacity"];
  lineOpacity = entityDefJson["lineOpacity"];
  hollow = entityDefJson["hollow"];
  color = entityDefJson["color"];
  renderMode = entityDefJson["renderMode"];

  const json& tilesetIdMaybe = entityDefJson["tilesetId"];
  if (!tilesetIdMaybe.is_null()) {
    tilesetId = tilesetIdMaybe;
  }

  tileRenderMode = entityDefJson["tileRenderMode"];
  // nineSliceBorders = jsonToVector<int>(entityDefJson["nineSliceBorders"]);
  pivotX = entityDefJson["pivotX"];
  pivotY = entityDefJson["pivotY"];
}
} // namespace LDTK
