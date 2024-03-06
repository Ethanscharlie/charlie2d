// tile.h
#ifndef TILE_H
#define TILE_H

#include "GameManager.h"
#include "Math.h"

// Components
#include "EntityBox.h"
#include "ResourceManager.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "Sprite.h"

class Entity;

struct TileRaw {
  std::string image;
  SDL_Rect srcRect;
  Box box;
};

struct TileGroup {
  TileGroup(){};

  TileGroup(TileRaw _tileRaw, Box newBox) {
    image = _tileRaw.image;
    srcRect = _tileRaw.srcRect;
    tileSize = _tileRaw.box.size.x;

    box = newBox;
  }

  ~TileGroup() { SDL_DestroyTexture(renderTexture); }

  SDL_Texture *render() {
    if (renderTexture != nullptr) {
      SDL_DestroyTexture(renderTexture);
      renderTexture = nullptr;
    }

    renderTexture =
        SDL_CreateTexture(GameManager::renderer, SDL_PIXELFORMAT_RGB888,
                          SDL_TEXTUREACCESS_TARGET, box.size.x, box.size.y);

    SDL_Texture *tileTexture =
        ResourceManager::getInstance(GameManager::renderer).getTexture(image);

    SDL_SetRenderTarget(GameManager::renderer, renderTexture);

    for (float y = 0; y < box.size.y; y += tileSize) {

      SDL_Rect boxRect = SDL_Rect(0, y, box.size.x, box.size.y);
      SDL_RenderCopy(GameManager::renderer, tileTexture, &srcRect, &boxRect);
    }

    SDL_SetRenderTarget(GameManager::renderer, nullptr);
    return renderTexture;
  }

  std::string image;
  SDL_Rect srcRect;
  Box box;
  int tileSize;

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
