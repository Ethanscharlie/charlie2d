#include "ldtk/LDTK_Tileset.hpp"

namespace LDTK {
Tileset::Tileset(const json &data, std::filesystem::path jsonDir) {
  int tileGridSize = data["tileGridSize"];
  int width = data["__cWid"];
  int height = data["__cHei"];

  auto jsonParentDir = jsonDir.parent_path();
  std::filesystem::path imageFileLocation;
  imageFileLocation = jsonParentDir / data["relPath"];

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < height; x++) {
      Image image = {imageFileLocation.string()};
      image.srcRect.x = x * tileGridSize;
      image.srcRect.y = y * tileGridSize;
      dictionary[TileLoc(x, y)] = image;
    }
  }
}
} // namespace LDTK
