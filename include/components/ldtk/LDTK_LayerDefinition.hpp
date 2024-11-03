#pragma once
#include "Entity.hpp"
#include "nlohmann/json.hpp"
#include <string>

using json = nlohmann::json;

namespace LDTK {
struct LayerDefinition {
  LayerDefinition(const json &layerDefJson, int _renderingLayer);
  ~LayerDefinition();

  Entity* renderingEntity;

  int renderingLayer = 0;

  std::string identifier;
  std::string type;
  int uid;
  int gridSize;
  int guideGridWid;
  int guideGridHei;
  int displayOpacity;
  int inactiveOpacity;
  bool hideInList;
  bool hideFieldsWhenInactive;
  bool canSelectWhenInactive;
  bool renderInWorldView;
  int pxOffsetX;
  int pxOffsetY;
  int parallaxFactorX;
  int parallaxFactorY;
  bool parallaxScaling;
  bool useAsyncRender;
  int autoSourceLayerDefUid;
  int tilesetDefUid = -1;
  int tilePivotX;
  int tilePivotY;
  int biomeFieldUid;
};
}
