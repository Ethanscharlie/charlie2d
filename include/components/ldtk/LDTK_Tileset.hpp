#pragma once
#include "Image.hpp"
#include "nlohmann/json.hpp"

namespace LDTK {
struct Tileset;
}

using json = nlohmann::json;
using TileLoc = std::pair<int, int>;
using AllTilesets = std::map<int, LDTK::Tileset>;

namespace LDTK {
struct Tileset {
  Tileset(const json &data, std::filesystem::path jsonDir);

  std::map<TileLoc, Image> dictionary;

  int tileGridSize;
  int width;
  int height;
};
} // namespace LDTK
