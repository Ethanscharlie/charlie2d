// tile.h
#ifndef TILE_H
#define TILE_H

#include "Camera.hpp"
#include "GameManager.hpp"
#include "Math.hpp"

// Components
#include "ResourceManager.hpp"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "Serializer.hpp"
#include "Sprite.hpp"
#include <vector>

class Entity;

struct TileRaw {
  std::string image;
  SDL_Rect srcRect;
  Box box;
};

struct TileGroup {
  TileGroup(std::vector<TileRaw> _tiles, Box newBox) {
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

    renderTexture =
        SDL_CreateTexture(GameManager::renderer, SDL_PIXELFORMAT_RGBA8888,
                          SDL_TEXTUREACCESS_TARGET, box.size.x, box.size.y);

    SDL_SetRenderTarget(GameManager::renderer, renderTexture);

    for (TileRaw rawTile : tiles) {
      SDL_Texture *tileTexture = Image(rawTile.image).texture;

      SDL_Rect boxRect = SDL_Rect(0, rawTile.box.position.y - box.position.y,
                                  rawTile.box.size.x, rawTile.box.size.y);

      SDL_RenderCopy(GameManager::renderer, tileTexture, &rawTile.srcRect,
                     &boxRect);
    }

    SDL_SetRenderTarget(GameManager::renderer, nullptr);

    return renderTexture;
  }

  SDL_Texture *getPreviousRender() { return renderTexture; }

  Box box;
  std::vector<TileRaw> tiles;

private:
  SDL_Texture *renderTexture = nullptr;
};

struct TileLayer {
  TileLayer() : name("") {}
  TileLayer(std::string _name, int _layer, std::vector<TileRaw> _tiles)
      : name(_name), layer(_layer) {
    tiles = _tiles;
  }
  std::vector<TileRaw> tiles;
  std::string name;
  int layer = 0;
};

std::vector<TileGroup> tileGroup(std::vector<TileRaw> &tiles, int tilesize);

/**
 * \brief Made to optimize tile rendering by not sorting each tile individualy
 */
class TileLayerComponent : public Component {
public:
  TileLayerComponent() : Component("TileLayer") {}

  void start() override;
  void update(float deltaTime) override;
  void onDestroy() override;

  std::vector<Entity *> tiles;
};

struct GridTile {
  GridTile(std::string _image, SDL_Rect _srcRect)
      : image(_image), srcRect(_srcRect) {}
  GridTile() {}
  std::string image = "";
  SDL_Rect srcRect = {0, 0, 16, 16};
};

struct TileGrid {
  void setTile(GridTile tile, int x, int y) {
    tiles[x][y] = tile;
    onModify();
  }

  void removeTile(int x, int y) {
    tiles[x].erase(y);
    onModify();
  }

  GridTile &getTile(int x, int y) { return tiles[x][y]; }

  std::vector<TileRaw> getTilesAsList() {
    std::vector<TileRaw> out;

    for (auto &[x, xlevel] : tiles) {
      for (auto &[y, tile] : xlevel) {
        TileRaw tileOut = TileRaw();
        tileOut.image = tile.image;
        tileOut.srcRect = tile.srcRect;
        tileOut.box = {(float)x * gridSize, (float)y * gridSize,
                       (float)gridSize, (float)gridSize};
        out.push_back(tileOut);
      }
    }

    return out;
  }

  void generateGroupedColliders() {
    premadeColliders.clear();
    std::vector<TileRaw> tiles = getTilesAsList();
    premadeColliders = tiles;
    // premadeColliders = tileGroup(tiles, gridSize);
  }

  void onModify() { generateGroupedColliders(); }

  std::vector<Box> getTileGroups() {
    std::vector<Box> out;

    for (auto &tile : premadeColliders) {
      out.push_back(tile.box);
    }

    return out;
  }

  bool checkCollision(Box box) {
    auto tileGroups = getTileGroups();
    for (Box &tileBox : tileGroups) {
      if (box.checkCollision(tileBox)) {
        return true;
      }
    }

    return false;
  }

  int gridSize = 16;
  std::map<int, std::map<int, GridTile>> tiles;

private:
  std::vector<TileRaw> premadeColliders;
};

class Tilemap : public Component {
public:
  Tilemap() {
    typeIsRendering = true;
    propertyRegister = {GET_PROP(tileGrid), GET_PROP(solid)};
  }

  Box getRenderBox(Box box) {
    Box renderBox;
    renderBox.position =
        (box.position - Camera::getPosition()) * Camera::getScale() +
        GameManager::gameWindowSize / 2;
    renderBox.size = box.size * Camera::getScale();
    return renderBox;
  }

  void start() override {}

  void update(float deltaTime) override { render(); }

  void render() {
    for (TileRaw &tile : tileGrid.getTilesAsList()) {
      Image tileImage;
      if (tile.image != "") {
        tileImage = tile.image;
      }

      if (tileImage.texture != nullptr) {
        Box renderBox = getRenderBox(tile.box);
        renderBox.size.x += 1;
        renderBox.size.y += 1;

        tileImage.srcRect = tile.srcRect;
        tileImage.render(renderBox);

        // SDL_RenderCopy(GameManager::renderer, tileImageTexture,
        // &tile.srcRect,
        //                &renderRect);
      }
    }
  }

  TileGrid tileGrid;
  bool solid = false;
  int gridSize = 16;
};
REGISTER_COMPONENT_TYPE(Tilemap);

#endif
