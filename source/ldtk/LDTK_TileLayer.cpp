#include "ldtk/LDTK_TileLayer.hpp"
#include "GameManager.hpp"
#include "ldtk/LDTK_LayerDefinition.hpp"
#include "ldtk/LDTK_Level.hpp"
#include "ldtk/LDTK_Project.hpp"

namespace LDTK {
LayerDefinition::~LayerDefinition() {
  if (renderingEntity) {
    renderingEntity->toDestroy = true;
  }
}

TileLayer::TileLayer(const json &data, Project* _project) {
  if (!data["visible"])
    return;
  if (data["__type"] != "AutoLayer" && data["__type"] != "Tiles") {
    std::cerr << "NOT A VALID TILE LAYER\n";
  }

  project = _project;

  opacity = data["__opacity"];
  iid = data["iid"];
  levelId = data["levelId"];
  layerDefUid = data["layerDefUid"];
  visible = data["visible"];
  seed = data["seed"];
  width = data["__cWid"];
  height = data["__cHei"];

  layerDefinition = project->layerDefinitions[layerDefUid].get();
  tileset = project->tilesets[layerDefinition->tilesetDefUid].get();

  json tilesList;
  if (data["__type"] == "Tiles") {
    tilesList = data["gridTiles"];
  } else if (data["__type"] == "AutoLayer") {
    tilesList = data["autoLayerTiles"];
  }

  int gridSize = layerDefinition->gridSize;

  for (const json &tileJson : tilesList) {
    TileLoc tilesetTile = {(int)tileJson["src"][0] / gridSize,
                           (int)tileJson["src"][1] / gridSize};

    auto pos = TileLoc((int)tileJson["px"][0] / gridSize,
                       (int)tileJson["px"][1] / gridSize);
    grid[pos] = (tilesetTile);
  }
}

void TileLayer::render(Level* level) {
  if (texture != nullptr) {
    SDL_DestroyTexture(texture);
  }

  int gridSize = layerDefinition->gridSize;

  texture = SDL_CreateTexture(GameManager::renderer, SDL_PIXELFORMAT_RGBA8888,
                              SDL_TEXTUREACCESS_TARGET, width * gridSize,
                              height * gridSize);

  SDL_SetRenderTarget(GameManager::renderer, texture);

  int pointerX = 0;
  int pointerY = 0;
  for (auto &[pos, tileLoc] : grid) {
    const Image &image = tileset->dictionary.at(tileLoc);

    SDL_Rect dstrect;
    dstrect.x = pos.first * gridSize;
    dstrect.y = pos.second * gridSize;
    dstrect.w = gridSize;
    dstrect.h = gridSize;

    Box box(dstrect.x, dstrect.y, gridSize, gridSize);
    box.position += level->levelBox.position;
    boxes.push_back(box);

    SDL_RenderCopy(GameManager::renderer, image.texture, &image.srcRect,
                   &dstrect);

    pointerX++;
    if (pointerX > width) {
      pointerX = 0;
      pointerY++;
    }
  }

  SDL_SetRenderTarget(GameManager::renderer, nullptr);
}

TileLayer::~TileLayer() {
  if (texture != nullptr) {
    SDL_DestroyTexture(texture);
  }
}
} // namespace LDTK
