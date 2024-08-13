#pragma once
#include "Box.hpp"
#include "SDL_render.h"
#include "ldtk/LDTK_LayerDefinition.hpp"
#include "ldtk/LDTK_Tileset.hpp"
#include "nlohmann/json.hpp"
#include <memory>
#include <string>
#include <vector>

using json = nlohmann::json;

namespace LDTK {
struct Level;
struct TileLayer {
  ~TileLayer();
  TileLayer(const json &data, LayerDefinition *_layerDefinition,
            Tileset *_tileset, Level *_level);
  void render();

  LayerDefinition *layerDefinition;
  Tileset *tileset;
  Level* level = nullptr;

  std::map<TileLoc, TileLoc> grid;

  SDL_Texture *texture = nullptr;
  std::vector<Box> boxes;

  int width;
  int height;
  int opacity;
  std::string iid;
  int levelId;
  int layerDefUid;
  bool visible;
  int seed;
};
} // namespace LDTK
