#include "ldtk.hpp"
#include "Entity.hpp"
#include "FadeTransition.hpp"
#include "GameManager.hpp"
#include "LDTKEntity.hpp"
#include "SDL_error.h"
#include "SDL_render.h"
#include "Tile.hpp"
#include <filesystem>
#include <utility>

Box LDTK::worldBox;
std::vector<Entity *> LDTK::entities;

json LDTK::fullJSON;
json LDTK::currentLevel;
std::map<std::string, std::map<std::string, json>> LDTK::worlds;

Entity *LDTK::ldtkPlayer = nullptr;
std::function<void(std::vector<Entity *>)> LDTK::onLoadLevel =
    [](std::vector<Entity *>) {};

std::map<std::string, std::map<std::string, TileLayer>> LDTK::preloadedTiles;

std::filesystem::path LDTK::jsonDir;

LDTK::LDTK() = default;

void LDTK::update(float deltaTime) {
  // if (checkPlayerOutsideBounds()) {
  //     loadLevel(findTraveledLevel());
  // }
}

void LDTK::unload(std::vector<Entity *> enlist) {
  if (ldtkPlayer != nullptr)
    ldtkPlayer->skipUpdate = true;
  for (Entity *entity : enlist) {
    if (entity == ldtkPlayer)
      continue;
    entities.erase(std::remove(entities.begin(), entities.end(), entity),
                   entities.end());

    if (entity == nullptr)
      continue;
    entity->toDestroy = true;
  }

  enlist.clear();
}

auto LDTK::findTraveledLevel(Entity *player) -> std::string {
  if (player == nullptr) {
    std::cout << "NO PLAYER SET!" << std::endl;
    return "";
  }

  for (json hit : currentLevel["__neighbours"]) {
    std::string iid = hit["levelIid"];
    json level = getLevelJson(iid);

    Box levelBox = {level["worldX"], level["worldY"], level["pxWid"],
                    level["pxHei"]};

    if (player->box.getLeft() < levelBox.getRight() &&
        player->box.getRight() > levelBox.getLeft() &&
        player->box.getTop() < levelBox.getBottom() &&
        player->box.getBottom() > levelBox.getTop()) {
      return iid;
    }
  }

  return "";
}

auto LDTK::checkOutsideBounds(Entity *player) -> bool {
  if (player->box.getCenter().x < worldBox.getRight() &&
      player->box.getCenter().x > worldBox.getLeft() &&
      player->box.getCenter().y < worldBox.getBottom() &&
      player->box.getCenter().y > worldBox.getTop()) {
    return false;
  }

  return true;
}

auto LDTK::getLevelJson(std::string iid) -> json {
  for (auto [worldName, levels] : worlds) {
    auto it = levels.find(iid);
    if (it != levels.end()) {
      json level = it->second;
      return level;
    }
  }

  std::cout << "Level Not Found" << "\n";
  return {};
}

void LDTK::loadTileLayer(json layer, int layerIndex, std::string iid) {
  int tileWidth = layer["__gridSize"];
  int tileHeight = layer["__gridSize"];

  std::string tileLayerJsonName;
  if (layer["__type"] == "Tiles") {
    tileLayerJsonName = "gridTiles";
  } else if (layer["__type"] == "AutoLayer") {
    std::cout << "AUTO\n";
    tileLayerJsonName = "autoLayerTiles";
  }

  std::vector<TileRaw> rawTiles;
  for (auto const &tile : layer[tileLayerJsonName]) {
    TileRaw rawTile;

    std::filesystem::path imageFileLocation;
    imageFileLocation =
        std::filesystem::path(jsonDir) / layer["__tilesetRelPath"];
    rawTile.image = imageFileLocation.string();

    rawTile.srcRect.x = tile["src"][0];
    rawTile.srcRect.y = tile["src"][1];
    rawTile.srcRect.w = layer["__gridSize"];
    rawTile.srcRect.h = layer["__gridSize"];

    rawTile.box = {tile["px"][0], tile["px"][1], layer["__gridSize"],
                   layer["__gridSize"]};

    rawTiles.push_back(rawTile);
  }

  std::string layerName = layer["__identifier"];
  preloadedTiles[iid][layerName] =
      TileLayer(layerName, layerIndex, rawTiles);

  // for (TileGroup &groupedTile : preloadedTiles[iid][layerName].tiles) {
  //   groupedTile.render();
  // }
}

void LDTK::preload(std::string iid) {
  auto thelevel = getLevelJson(iid);
  auto &layerInstances = thelevel["layerInstances"];

  for (auto it = layerInstances.rbegin(); it != layerInstances.rend(); ++it) {
    auto const &layer = *it;
    if (layer["__type"] == "Tiles" || layer["__type"] == "AutoLayer") {
      loadTileLayer(layer, it - layerInstances.rbegin(), iid);
    }
  }
}

void LDTK::loadLevel(std::string iid, bool handleUnload) {
  if (iid == "")
    return;

  if (preloadedTiles.find(iid) == preloadedTiles.end()) {
    preload(iid);
  }

  // GameManager::updateEntities = false;
  std::vector<Entity *> lastEntities = entities;

  currentLevel = getLevelJson(iid);
  auto &layerInstances = currentLevel["layerInstances"];

  worldBox = {currentLevel["worldX"], currentLevel["worldY"],
              currentLevel["pxWid"], currentLevel["pxHei"]};

  std::cout << "load level with id " << iid << "\n";
  for (auto &[layerName, tileLayer] : preloadedTiles[iid]) {
    Entity *layerObject = GameManager::createEntity(layerName);
    layerObject->add<TileLayerComponent>();
    layerObject->useLayer = true;
    layerObject->layer = tileLayer.layer;

    entities.push_back(layerObject);

    std::cout << "Loading layer " << layerName << "\n";

    for (TileRaw &groupedTile : tileLayer.tiles) {
      Entity *tile = GameManager::createEntity(layerName);

      tile->box.position = (groupedTile.box.position);
      tile->box.size = (groupedTile.box.size);
      tile->box.position += (worldBox.position);

      tile->add<Sprite>()->image = groupedTile.image;
      tile->add<Sprite>()->image.srcRect = groupedTile.srcRect;

      tile->active = false;
      layerObject->get<TileLayerComponent>()->tiles.push_back(tile);

      entities.push_back(tile);
    }
  }

  for (auto it = layerInstances.rbegin(); it != layerInstances.rend(); ++it) {
    auto const &layer = *it;

    if (layer["__type"] == "Entities") {
      for (auto const &entity : layer["entityInstances"]) {
        Entity *object = GameManager::createEntity(entity["__identifier"]);

        object->add<Sprite>();
        object->add<LDTKEntity>();

        object->get<LDTKEntity>()->entityJson = entity;

        object->box.position = {entity["px"][0], entity["px"][1]};
        object->box.position += (worldBox.position);

        object->box.size = {entity["width"], entity["height"]};

        entities.push_back(object);
      }
    }
  }

  onLoadLevel(entities);

  if (handleUnload) {
    unload(lastEntities);
    Camera::cameraLimitBox = worldBox;
    Camera::setPosition(worldBox.getCenter());
  } else {

    Camera::cameraLimitBox = worldBox;
  }
}

void LDTK::loadJson(std::string file) {
  std::ifstream f(file);
  fullJSON = json::parse(f);
  jsonDir = std::filesystem::path(file).parent_path();

  auto flags = fullJSON["flags"];
  bool isMultiworld =
      std::find(flags.begin(), flags.end(), "MultiWorlds") != flags.end();
  if (isMultiworld) {
    for (json world : fullJSON["worlds"]) {
      for (json level : world["levels"]) {
        worlds[world["identifier"]][level["iid"]] = level;
      }
    }

  } else {
    for (json level : fullJSON["levels"]) {
      worlds["World"][level["iid"]] = level;
    }
  }
}
