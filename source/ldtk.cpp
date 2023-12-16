#include "ldtk.h"
#include "Entity.h"
#include "GameManager.h"

Box LDTK::worldBox;
std::vector<Entity *> LDTK::entities;

json LDTK::fullJSON;
json LDTK::currentLevel;
std::map<std::string, json> LDTK::levels;

Entity *LDTK::ldtkPlayer = nullptr;
std::function<void()> LDTK::onLoadLevel = []() {};

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
    if (entity == nullptr)
      continue;
    if (entity == ldtkPlayer)
      continue;
    entities.erase(std::remove(entities.begin(), entities.end(), entity),
                   entities.end());

    if (std::find(GameManager::getAllObjects().begin(),
                  GameManager::getAllObjects().end(),
                  entity) == GameManager::getAllObjects().end()) {
      continue;
    }
    // std::cout << entity->tag << std::endl;
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
    json level = levels[iid];

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

void LDTK::loadLevel(std::string iid) {
  if (iid == "")
    return;
  // GameManager::updateEntities = false;
  std::vector<Entity *> lastEntities = entities;

  currentLevel = levels[iid];
  auto &layerInstances = currentLevel["layerInstances"];

  worldBox = {currentLevel["worldX"], currentLevel["worldY"],
              currentLevel["pxWid"], currentLevel["pxHei"]};

  for (auto it = layerInstances.rbegin(); it != layerInstances.rend(); ++it) {
    auto const &layer = *it;
    if (layer["__type"] == "Tiles") {
      int tileWidth = layer["__gridSize"];
      int tileHeight = layer["__gridSize"];

      Entity *layerObject = GameManager::createEntity(layer["__identifier"]);
      layerObject->require<TileLayer>();
      layerObject->get<TileLayer>()->useLayer = true;

      entities.push_back(layerObject);

      for (auto const &tile : layer["gridTiles"]) {
        Entity *tileObject = GameManager::createEntity(layer["__identifier"]);

        tileObject->require<Sprite>();
        tileObject->require<LDTKEntity>();

        tileObject->get<LDTKEntity>()->entityJson = tile;

        std::string imageFileLocation;

        imageFileLocation.append("img/ldtk");
        imageFileLocation.append("/");
        imageFileLocation.append(layer["__tilesetRelPath"]);

        tileObject->get<Sprite>()->loadTexture(imageFileLocation);

        tileObject->get<Sprite>()->sourceRect.x = tile["src"][0];
        tileObject->get<Sprite>()->sourceRect.y = tile["src"][1];
        tileObject->get<Sprite>()->sourceRect.w = layer["__gridSize"];
        tileObject->get<Sprite>()->sourceRect.h = layer["__gridSize"];

        tileObject->get<Sprite>()->standardUpdate = false;

        tileObject->require<entityBox>()->setSize(
            {layer["__gridSize"], layer["__gridSize"]});

        tileObject->require<entityBox>()->setPosition(
            {tile["px"][0], tile["px"][1]});
        tileObject->require<entityBox>()->changePosition(worldBox.position);

        tileObject->require<entityBox>()->setSize(
            {static_cast<float>(tileWidth), static_cast<float>(tileHeight)});

        layerObject->get<TileLayer>()->tiles.push_back(tileObject);

        // entities.push_back(tileObject);
      }
    }

    else if (layer["__type"] == "Entities") {
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
  GameManager::updateEntities = false;
  GameManager::doFade(
      [lastEntities]() {
        Camera::cameraLimitBox = worldBox;
        unload(lastEntities);
        GameManager::updateEntities = true;
      },
      150);
}

void LDTK::loadJson(std::string file) {
  std::ifstream f(file);
  fullJSON = json::parse(f);

  for (json level : fullJSON["levels"]) {
    levels[level["iid"]] = level;
  }
}
