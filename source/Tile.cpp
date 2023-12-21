#include "Tile.h"
#include "Sprite.h"
#include "Entity.h"

void TileLayer::start() {
    entity->useLayer = true;
}

void TileLayer::update(float deltaTime) {
    for (Entity* tile : tiles) {
        tile->update();
    }
}

void TileLayer::onDestroy() {
    for (Entity* tile : tiles) {
        tile->toDestroy = true;
    }
}
