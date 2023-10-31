#include "ldtk.h"
#include "Entity.h"

Box LDTK::worldBox;
std::vector<Entity*> LDTK::entities;

json LDTK::fullJSON;
json LDTK::currentLevel;
std::map<std::string, json> LDTK::levels;

Entity* LDTK::ldtkPlayer = nullptr;
std::function<void()> LDTK::onLoadLevel = [](){};

LDTK::LDTK() {}

void LDTK::update(float deltaTime) {
    //if (checkPlayerOutsideBounds()) {
    //    loadLevel(findTraveledLevel());
    //}
}

void LDTK::unload(std::vector<Entity*> enlist) {
    if (ldtkPlayer != nullptr) ldtkPlayer->skipUpdate = true;
    for (Entity* entity : enlist) {
        if (entity == ldtkPlayer) continue;
        entity->toDestroy = true;
    }

    enlist.clear();
}

std::string LDTK::findTraveledLevel(Entity* player) {
    if (player == nullptr) { std::cout << "NO PLAYER SET!" << std::endl;
        return "";
    }

    for (json hit : currentLevel["__neighbours"]) {
        std::string iid = hit["levelIid"];
        json level = levels[iid];

        Box levelBox = {level["worldX"], level["worldY"], level["pxWid"], level["pxHei"]};

        if (player->box->getBox().getLeft()   < levelBox.getRight() && 
                player->box->getBox().getRight()  > levelBox.getLeft() && 
                player->box->getBox().getTop()    < levelBox.getBottom() && 
                player->box->getBox().getBottom() > levelBox.getTop()) 
        {
            return iid;
        }
    }

    return "";
}

bool LDTK::checkOutsideBounds(Entity* player) {
    if (player->box->getBox().getCenter().x < worldBox.getRight() && 
            player->box->getBox().getCenter().x > worldBox.getLeft() && 
            player->box->getBox().getCenter().y < worldBox.getBottom() && 
            player->box->getBox().getCenter().y > worldBox.getTop()) 
    {
        return false;
    }

    return true;
}

void LDTK::loadLevel(std::string iid, Scene* scene) {
    if (iid == "") return;
    //scene->updateEntities = false;
    std::vector<Entity*> lastEntities = entities;

    currentLevel = levels[iid];
    auto& layerInstances = currentLevel["layerInstances"];

    worldBox = {currentLevel["worldX"], currentLevel["worldY"], currentLevel["pxWid"], currentLevel["pxHei"]};

    for (auto it = layerInstances.rbegin(); it != layerInstances.rend(); ++it) { 
        auto const& layer = *it;
        if (layer["__type"] == "Tiles") {
            int tileWidth = layer ["__gridSize"];
            int tileHeight = layer["__gridSize"];

            Entity* layerObject = scene->createEntity(layer["__identifier"]);
            layerObject->addComponent<TileLayer>();
            layerObject->getComponent<TileLayer>().useLayer = true;
            entities.push_back(layerObject);

            for (auto const& tile : layer["gridTiles"]) {
                Entity* tileObject = scene->createEntity(layer["__identifier"]);

                tileObject->addComponent<Sprite>();
                tileObject->addComponent<LDTKEntity>();
                tileObject->getComponent<LDTKEntity>().entityJson = tile;

                std::string imageFileLocation;

                imageFileLocation.append("img/ldtk"); 
                imageFileLocation.append("/"); 
                imageFileLocation.append(layer["__tilesetRelPath"]);

                tileObject->getComponent<Sprite>().loadTexture(imageFileLocation);

                tileObject->getComponent<Sprite>().sourceRect.x = tile["src"][0];
                tileObject->getComponent<Sprite>().sourceRect.y = tile["src"][1];
                tileObject->getComponent<Sprite>().sourceRect.w = layer["__gridSize"];
                tileObject->getComponent<Sprite>().sourceRect.h = layer["__gridSize"];

                tileObject->getComponent<Sprite>().standardUpdate = false;

                tileObject->box->setSize({layer["__gridSize"], layer["__gridSize"]});

                tileObject->box->setPosition({tile["px"][0], tile["px"][1]});
                tileObject->box->changePosition(worldBox.position);

                tileObject->box->setSize({static_cast<float>(tileWidth), static_cast<float>(tileHeight)});

                layerObject->getComponent<TileLayer>().tiles.push_back(tileObject);

                //entities.push_back(tileObject);
            }
        } 

        else if (layer["__type"] == "Entities") {
            for (auto const& entity : layer["entityInstances"]) {
                Entity* object = scene->createEntity(entity["__identifier"]);

                object->addComponent<Sprite>();
                object->addComponent<LDTKEntity>();
                object->getComponent<LDTKEntity>().entityJson = entity;

                object->box->setPosition({entity["px"][0], entity["px"][1]});
                object->box->changePosition(worldBox.position);

                object->box->setSize({entity["width"], entity["height"]});

                entities.push_back(object);
            }
        }
    }

    onLoadLevel();
    scene->updateEntities = false;
    GameManager::doFade([lastEntities, scene]() {
        GameManager::cameraLimitBox = worldBox;
        unload(lastEntities);
        scene->updateEntities = true;
    }, 150);
}

void LDTK::loadJson(std::string file) {
    std::ifstream f(file);
    fullJSON = json::parse(f);

    for (json level : fullJSON["levels"]) {
        levels[level["iid"]] = level;
    }
}
