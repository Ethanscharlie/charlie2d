#include "Tile.h"

void TileLayer::start() {
    useLayer = true;
}

void TileLayer::update(float deltaTime) {
    for (Entity* tile : tiles) {
        tile->getComponent<Sprite>().update(deltaTime);
    }
}

void TileLayer::onDestroy() {
    for (Entity* tile : tiles) {
        tile->toDestroy = true;
    }
}
