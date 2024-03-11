#include "Tile.h"
#include "Entity.h"
#include "Sprite.h"

std::vector<TileGroup> tileGroup(std::vector<TileRaw> &tiles, int tilesize) {
  std::vector<TileGroup> aabbPool;
  std::vector<std::pair<Vector2f, std::vector<TileRaw>>> xMap;

  for (const auto &tile : tiles) {
    Box box = tile.box;
    if (box.size.x != tilesize) {
      aabbPool.push_back(TileGroup(std::vector<TileRaw>{tile}, box));
    } else {
      bool foundX = false;
      for (auto &entry : xMap) {
        if (entry.first.x == box.position.x) {
          entry.second.push_back(tile);
          foundX = true;
          break;
        }
      }
      if (!foundX) {
        xMap.emplace_back(box.position, std::vector<TileRaw>{tile});
      }
    }
  }

  for (auto &entry : xMap) {
    auto &curList = entry.second;
    if (!curList.empty()) {
      float curY = curList[0].box.position.y;
      int curH = 1;
      for (size_t i = 1; i < curList.size(); ++i) {
        if (curY + curH * tilesize == curList[i].box.position.y) {
          ++curH;
        } else {
          aabbPool.emplace_back(TileGroup(
              entry.second, {entry.first.x, curY, (float)tilesize, (float)curH * tilesize}));
          curY = curList[i].box.position.y;
          curH = 1;
        }
      }
      aabbPool.emplace_back(
          TileGroup(entry.second, {entry.first.x, curY, (float)tilesize, (float)curH * tilesize}));

    }
  }

  return aabbPool;
}

void TileLayerComponent::start() { entity->useLayer = true; }

void TileLayerComponent::update(float deltaTime) {
  for (Entity *tile : tiles) {
    tile->update();
  }
}

void TileLayerComponent::onDestroy() {
  for (Entity *tile : tiles) {
    tile->toDestroy = true;
  }
}
