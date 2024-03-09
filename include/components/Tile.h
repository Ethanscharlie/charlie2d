// tile.h
#ifndef TILE_H
#define TILE_H

#include "GameManager.h"
#include "Math.h"

// Components
#include "EntityBox.h"
#include "ResourceManager.h"
#include "SDL_blendmode.h"
#include "SDL_error.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "Sprite.h"
#include <numeric>
#include <vector>

class Entity;

struct TileRaw {
  std::string image;
  SDL_Rect srcRect;
  Box box;
};

struct TileGroup {
  TileGroup(std::vector<TileRaw> _tiles, Box newBox) {
    // image = _tileRaw.image;
    // srcRect = _tileRaw.srcRect;
    tiles = _tiles;

    box = newBox;
  }

  ~TileGroup() {
    if (renderTexture != nullptr) {
      SDL_DestroyTexture(renderTexture);
    }
  }

  SDL_Texture *render() {
    if (renderTexture != nullptr) {
      SDL_DestroyTexture(renderTexture);
    }

    SDL_Texture *renderTexture =
        SDL_CreateTexture(GameManager::renderer, SDL_PIXELFORMAT_RGBA8888,
                          SDL_TEXTUREACCESS_TARGET, box.size.x, box.size.y);

    SDL_SetRenderTarget(GameManager::renderer, renderTexture);

    for (TileRaw rawTile : tiles) {
      SDL_Texture *tileTexture =
          ResourceManager::getInstance(GameManager::renderer)
              .getTexture(rawTile.image);

      SDL_Rect boxRect = SDL_Rect(0, rawTile.box.position.y - box.position.y,
                                  rawTile.box.size.x, rawTile.box.size.y);

      SDL_RenderCopy(GameManager::renderer, tileTexture, &rawTile.srcRect,
                     &boxRect);
    }

    SDL_SetRenderTarget(GameManager::renderer, nullptr);

    return renderTexture;
  }

  Box box;
  std::vector<TileRaw> tiles;

private:
  SDL_Texture *renderTexture = nullptr;
};

std::vector<TileGroup> tileGroup(std::vector<TileRaw> &tiles);

/**
 * \brief Made to optimize tile rendering by not sorting each tile individualy
 */
class TileLayer : public Component {
public:
  TileLayer() : Component("TileLayer") {}

  void start() override;
  void update(float deltaTime) override;
  void onDestroy() override;

  std::vector<Entity *> tiles;
};

#endif
