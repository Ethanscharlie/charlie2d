#include "Tile.h"
#include "Sprite.h"
#include "Entity.h"

void TileLayer::start() {
    useLayer = true;
}

void TileLayer::update(float deltaTime) {
    for (Entity* tile : tiles) {
        tile->get<Sprite>()->update(deltaTime);
    }
}

void TileLayer::onDestroy() {
    for (Entity* tile : tiles) {
        tile->toDestroy = true;
    }
}
