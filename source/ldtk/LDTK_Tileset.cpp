#include "ldtk/LDTK_Tileset.hpp"

namespace LDTK {
Tileset::Tileset(const json &data, std::filesystem::path jsonDir) {
  tileGridSize = data["tileGridSize"];
  width = data["__cWid"];
  height = data["__cHei"];

  auto jsonParentDir = jsonDir.parent_path();
  std::filesystem::path imageFileLocation;
  imageFileLocation = jsonParentDir / data["relPath"];

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      Image image = {imageFileLocation.string()};
      image.srcRect.x = x * tileGridSize;
      image.srcRect.y = y * tileGridSize;
      image.srcRect.w = tileGridSize;
      image.srcRect.h = tileGridSize;
      dictionary[TileLoc(x, y)] = image;
    }
  }
}
} // namespace LDTK
