#include "Tile.h"
#include "Entity.h"
#include "Sprite.h"

std::vector<TileGroup> tileGroup(std::vector<TileRaw> &tiles) {
  std::vector<TileGroup> aabbPool;
  std::vector<std::pair<float, std::vector<float>>> xMap;

  for (const auto &tile : tiles) {
    Box box = tile.box;
    if (box.size.x != 16) {
      aabbPool.push_back(TileGroup(tile, box));
    } else {
      bool foundX = false;
      for (auto &entry : xMap) {
        if (entry.first == box.position.x) {
          entry.second.push_back(box.position.y);
          foundX = true;
          break;
        }
      }
      if (!foundX) {
        xMap.emplace_back(box.position.x, std::vector<float>{box.position.y});
      }
    }
  }

  for (auto &entry : xMap) {
    auto &curList = entry.second;
    if (!curList.empty()) {
      float curY = curList[0];
      int curH = 1;
      for (size_t i = 1; i < curList.size(); ++i) {
        if (curY + curH * 16 == curList[i]) {
          ++curH;
        } else {
          aabbPool.emplace_back(
              TileGroup(tiles[0], {entry.first, curY, 16, (float)curH * 16}));
          curY = curList[i];
          curH = 1;
        }
      }
      aabbPool.emplace_back(
          TileGroup(tiles[0], {entry.first, curY, 16, (float)curH * 16}));
    }
  }

  return aabbPool;
}

void TileLayer::start() { entity->useLayer = true; }

void TileLayer::update(float deltaTime) {
  for (Entity *tile : tiles) {
    tile->update();
  }
}

void TileLayer::onDestroy() {
  for (Entity *tile : tiles) {
    tile->toDestroy = true;
  }
}
