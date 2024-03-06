#include "ldtk.h"
#include "Entity.h"
#include "FadeTransition.h"
#include "GameManager.h"
#include "LDTKEntity.h"
#include "Tile.h"

Box LDTK::worldBox;
std::vector<Entity *> LDTK::entities;

json LDTK::fullJSON;
json LDTK::currentLevel;
std::map<std::string, std::map<std::string, json>> LDTK::worlds;

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

void LDTK::loadLevel(std::string iid, bool handleUnload) {
  std::cout << "staring to load level 117\n";

  if (iid == "")
    return;
  // GameManager::updateEntities = false;
  std::vector<Entity *> lastEntities = entities;

  currentLevel = getLevelJson(iid);
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
      layerObject->useLayer = true;

      entities.push_back(layerObject);

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

        // Entity *tileObject =
        // GameManager::createEntity(layer["__identifier"]);
        //
        // tileObject->require<Sprite>();
        // tileObject->require<LDTKEntity>();
        //
        // tileObject->get<LDTKEntity>()->entityJson = tile;
        //
        // std::string imageFileLocation;
        //
        // imageFileLocation.append("img/ldtk");
        // imageFileLocation.append("/");
        // imageFileLocation.append(layer["__tilesetRelPath"]);
        //
        // tileObject->get<Sprite>()->loadTexture(imageFileLocation);
        //
        // tileObject->get<Sprite>()->sourceRect.x = tile["src"][0];
        // tileObject->get<Sprite>()->sourceRect.y = tile["src"][1];
        // tileObject->get<Sprite>()->sourceRect.w = layer["__gridSize"];
        // tileObject->get<Sprite>()->sourceRect.h = layer["__gridSize"];
        //
        // tileObject->active = false;
        //
        // tileObject->require<entityBox>()->setSize(
        //     {layer["__gridSize"], layer["__gridSize"]});
        //
        // tileObject->require<entityBox>()->setPosition(
        //     {tile["px"][0], tile["px"][1]});
        // tileObject->require<entityBox>()->changePosition(worldBox.position);
        //
        // tileObject->require<entityBox>()->setSize(
        //     {static_cast<float>(tileWidth), static_cast<float>(tileHeight)});
        //
        // layerObject->get<TileLayer>()->tiles.push_back(tileObject);
      }

      for (TileGroup groupedTile : tileGroup(rawTiles)) {
        Entity *tile = GameManager::createEntity(layer["__identifier"]);

        tile->box->setPosition(groupedTile.box.position);
        tile->box->setSize(groupedTile.box.size);
        tile->box->changePosition(worldBox.position);

        // tile->add<Sprite>()->loadTexture(groupedTile.image, false);
        tile->add<Sprite>()->texture = groupedTile.render();
        // tile->add<Sprite>()->sourceRect = groupedTile.srcRect;

        tile->get<Sprite>()->showBorders = true;

        tile->active = false;
        layerObject->get<TileLayer>()->tiles.push_back(tile);
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

  if (handleUnload) {
    // GameManager::updateEntities = false;
    FadeTransition *Fader = nullptr;
    if (GameManager::getComponents<FadeTransition>().size() > 0) {
      Fader = GameManager::getComponents<FadeTransition>()[0];
      // std::cout << "There was a fader\n";
    } else {
      Fader = GameManager::createEntity("Fader")->add<FadeTransition>();
      // std::cout << "There was not a fader\n";
    }

    // std::cout << "Tag: " << Fader->entity->tag << "\n";

    // Fader->doFade(
    //     [lastEntities]() {
    //       std::cout << "On middle ran\n";
    //       Camera::cameraLimitBox = worldBox;
    //       unload(lastEntities);
    //       GameManager::updateEntities = true;
    //     },
    //     150, [lastEntities]() {});
    Camera::cameraLimitBox = worldBox;
    Camera::setPosition(worldBox.getCenter());
    std::cout << "preparing to unload 117\n";
    unload(lastEntities);
    std::cout << "finished unload 117\n";
    GameManager::updateEntities = true;
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
