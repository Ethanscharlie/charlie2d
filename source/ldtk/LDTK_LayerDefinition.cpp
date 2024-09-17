#include "ldtk/LDTK_LayerDefinition.hpp"
#include "Entity.hpp"
#include "ldtk/LDTK_Tilemap.hpp"

namespace LDTK {
LayerDefinition::LayerDefinition(const json &layerDefJson, int _renderingLayer) {
  identifier = layerDefJson["identifier"];
  type = layerDefJson["__type"];
  uid = layerDefJson["uid"];
  gridSize = layerDefJson["gridSize"];

  // guideGridWid = layerDefJson["guideGridWid"];
  // guideGridHei = layerDefJson["guideGridHei"];
  // displayOpacity = layerDefJson["displayOpacity"];
  // inactiveOpacity = layerDefJson["inactiveOpacity"];
  // hideInList = layerDefJson["hideInList"];
  // hideFieldsWhenInactive = layerDefJson["hideFieldsWhenInactive"];
  // canSelectWhenInactive = layerDefJson["canSelectWhenInactive"];
  // renderInWorldView = layerDefJson["renderInWorldView"];
  // pxOffsetX = layerDefJson["pxOffsetX"];
  // pxOffsetY = layerDefJson["pxOffsetY"];
  // parallaxFactorX = layerDefJson["parallaxFactorX"];
  // parallaxFactorY = layerDefJson["parallaxFactorY"];
  // parallaxScaling = layerDefJson["parallaxScaling"];
  // useAsyncRender = layerDefJson["useAsyncRender"];
  // autoSourceLayerDefUid = layerDefJson["autoSourceLayerDefUid"];
  if (!layerDefJson["tilesetDefUid"].is_null()) {
    tilesetDefUid = layerDefJson["tilesetDefUid"];
  }
  // tilePivotX = layerDefJson["tilePivotX"];
  // tilePivotY = layerDefJson["tilePivotY"];
  // biomeFieldUid = layerDefJson["biomeFieldUid"];
  //
  renderingLayer = _renderingLayer;

  renderingEntity = GameManager::createEntity(identifier);
  renderingEntity->add<Tilemap>();
}
} // namespace LDTK
