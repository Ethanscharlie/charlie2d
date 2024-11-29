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
struct Project;

struct TileLayer {
  ~TileLayer();
  TileLayer(const json &data, Project* _project);
  void render(Level* level);

  Project* project = nullptr;

  LayerDefinition *layerDefinition;
  Tileset *tileset;

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
