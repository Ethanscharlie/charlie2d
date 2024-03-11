#include "ldtk.h"
#include "Entity.h"
#include "FadeTransition.h"
#include "GameManager.h"
#include "LDTKEntity.h"
#include "SDL_error.h"
#include "SDL_render.h"
#include "Tile.h"
#include <utility>

Box LDTK::worldBox;
std::vector<Entity *> LDTK::entities;

json LDTK::fullJSON;
json LDTK::currentLevel;
std::map<std::string, std::map<std::string, json>> LDTK::worlds;

Entity *LDTK::ldtkPlayer = nullptr;
std::function<void()> LDTK::onLoadLevel = []() {};

std::map<std::string, std::map<std::string, TileLayer>> LDTK::preloadedTiles;

LDTK::LDTK() {}

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

std::string LDTK::findTraveledLevel(Entity *player) {
  if (player == nullptr) {
    std::cout << "NO PLAYER SET!" << std::endl;
    return "";
  }

  for (json hit : currentLevel["__neighbours"]) {
    std::string iid = hit["levelIid"];
    json level = getLevelJson(iid);

    Box levelBox = {level["worldX"], level["worldY"], level["pxWid"],
                    level["pxHei"]};

    if (player->require<entityBox>()->getBox().getLeft() <
            levelBox.getRight() &&
        player->require<entityBox>()->getBox().getRight() >
            levelBox.getLeft() &&
        player->require<entityBox>()->getBox().getTop() <
            levelBox.getBottom() &&
        player->require<entityBox>()->getBox().getBottom() >
            levelBox.getTop()) {
      return iid;
    }
  }

  return "";
}

bool LDTK::checkOutsideBounds(Entity *player) {
  if (player->require<entityBox>()->getBox().getCenter().x <
          worldBox.getRight() &&
      player->require<entityBox>()->getBox().getCenter().x >
          worldBox.getLeft() &&
      player->require<entityBox>()->getBox().getCenter().y <
          worldBox.getBottom() &&
      player->require<entityBox>()->getBox().getCenter().y >
          worldBox.getTop()) {
    return false;
  }

  return true;
}

json LDTK::getLevelJson(std::string iid) {
  for (auto [worldName, levels] : worlds) {
    auto it = levels.find(iid);
    if (it != levels.end()) {
      json level = it->second;
      return level;
    }
  }

  std::cout << "Level Not Found"
            << "\n";
  return {};
}

void LDTK::preload(std::string iid) {
  auto thelevel = getLevelJson(iid);
  auto &layerInstances = thelevel["layerInstances"];

  for (auto it = layerInstances.rbegin(); it != layerInstances.rend(); ++it) {
    auto const &layer = *it;
    if (layer["__type"] == "Tiles") {
      int tileWidth = layer["__gridSize"];
      int tileHeight = layer["__gridSize"];

      std::vector<TileRaw> rawTiles;
      for (auto const &tile : layer["gridTiles"]) {
        TileRaw rawTile;

        std::string imageFileLocation;
        imageFileLocation.append("img/ldtk");
        imageFileLocation.append("/");
        imageFileLocation.append(layer["__tilesetRelPath"]);
        rawTile.image = imageFileLocation;

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
          TileLayer(layerName, tileGroup(rawTiles, layer["__gridSize"]));

      for (TileGroup &groupedTile : preloadedTiles[iid][layerName].tiles) {
        groupedTile.render();
      }
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
    layerObject->require<TileLayerComponent>();
    layerObject->useLayer = true;

    entities.push_back(layerObject);

    std::cout << "Loading layer " << layerName << "\n";

    for (TileGroup &groupedTile : tileLayer.tiles) {
      Entity *tile = GameManager::createEntity(layerName);

      tile->box->setPosition(groupedTile.box.position);
      tile->box->setSize(groupedTile.box.size);
      tile->box->changePosition(worldBox.position);

      tile->add<Sprite>()->texture = groupedTile.getPreviousRender();
      tile->get<Sprite>()->showBorders = true;

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

        object->require<Sprite>();
        object->require<LDTKEntity>();

        object->get<LDTKEntity>()->entityJson = entity;

        object->require<entityBox>()->setPosition(
            {entity["px"][0], entity["px"][1]});
        object->require<entityBox>()->changePosition(worldBox.position);

        object->require<entityBox>()->setSize(
            {entity["width"], entity["height"]});

        entities.push_back(object);
      }
    }
  }

  onLoadLevel();

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
