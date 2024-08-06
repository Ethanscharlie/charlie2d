#include "ldtk/LDTK_EntityDefinition.hpp"

namespace LDTK {
EntityDefinition::EntityDefinition(const json &entityDefJson) {
  identifier = entityDefJson["identifier"];
  uid = entityDefJson["uid"];
  // tags = entityDefJson["tags"]; // TODO LIKEY ERROR
  // allowOutOfBounds = entityDefJson["allowOutOfBounds"];
  width = entityDefJson["width"];
  height = entityDefJson["height"];
  // tileOpacity = entityDefJson["tileOpacity"];
  // fillOpacity = entityDefJson["fillOpacity"];
  // lineOpacity = entityDefJson["lineOpacity"];
  // hollow = entityDefJson["hollow"];
  // color = entityDefJson["color"];
  // renderMode = entityDefJson["renderMode"];
  // tilesetId = entityDefJson["tilesetId"]; // TODO CAN BE NULL
  // tileRenderMode = entityDefJson["tileRenderMode"];
  // nineSliceBorders = jsonToVector<int>(entityDefJson["nineSliceBorders"]);
  // pivotX = entityDefJson["pivotX"];
  // pivotY = entityDefJson["pivotY"];
}
} // namespace LDTK
