//tile.h
#ifndef TILE_H
#define TILE_H

#include "Math.h"
#include "ResourceManager.h"
#include "GameManager.h"

// Components
#include "EntityBox.h"
#include "Sprite.h"

class Entity;

/**
 * \brief Made to optimize tile rendering by not sorting each tile individualy
 */
class TileLayer : public Component {
    public:
    TileLayer() : Component("TileLayer") {} 

    void start() override;
    void update(float deltaTime) override;
    void onDestroy() override;

    std::vector<Entity*> tiles;
};

#endif
