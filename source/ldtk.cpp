#include "ldtk.hpp"
#include "Entity.hpp"
#include "FadeTransition.hpp"
#include "GameManager.hpp"
#include "LDTKEntity.hpp"
#include "SDL_error.h"
#include "SDL_render.h"
#include <filesystem>
#include <utility>

namespace LDTK {

} // namespace LDTK

// class Sprite;
//
// Box LDTK::worldBox;
// std::vector<Entity *> LDTK::entities;
//
// json LDTK::fullJSON;
// json LDTK::currentLevel;
// std::map<std::string, std::map<std::string, json>> LDTK::worlds;
//
// Entity *LDTK::ldtkPlayer = nullptr;
// std::function<void(std::vector<Entity *>)> LDTK::onLoadLevel =
//     [](std::vector<Entity *>) {};
//
// std::map<std::string, std::map<std::string, TileLayer>> LDTK::preloadedTiles;
//
// std::filesystem::path LDTK::jsonDir;
//
// LDTK::LDTK() = default;
//
// void LDTK::update(float deltaTime) {
//   // if (checkPlayerOutsideBounds()) {
//   //     loadLevel(findTraveledLevel());
//   // }
// }
//
// void LDTK::unload(std::vector<Entity *> enlist) {
//   if (ldtkPlayer != nullptr)
//     ldtkPlayer->skipUpdate = true;
//   for (Entity *entity : enlist) {
//     if (entity == ldtkPlayer)
//       continue;
//     entities.erase(std::remove(entities.begin(), entities.end(), entity),
//                    entities.end());
//
//     if (entity == nullptr)
//       continue;
//     entity->toDestroy = true;
//   }
//
//   enlist.clear();
// }
//
// auto LDTK::findTraveledLevel(Entity *player) -> std::string {
//   if (player == nullptr) {
//     std::cout << "NO PLAYER SET!" << std::endl;
//     return "";
//   }
//
//   for (json hit : currentLevel["__neighbours"]) {
//     std::string iid = hit["levelIid"];
//     json level = getLevelJson(iid);
//
//     Box levelBox = {level["worldX"], level["worldY"], level["pxWid"],
//                     level["pxHei"]};
//
//     if (player->box.getLeft() < levelBox.getRight() &&
//         player->box.getRight() > levelBox.getLeft() &&
//         player->box.getTop() < levelBox.getBottom() &&
//         player->box.getBottom() > levelBox.getTop()) {
//       return iid;
//     }
//   }
//
//   return "";
// }
//
// auto LDTK::checkOutsideBounds(Entity *player) -> bool {
//   if (player->box.getCenter().x < worldBox.getRight() &&
//       player->box.getCenter().x > worldBox.getLeft() &&
//       player->box.getCenter().y < worldBox.getBottom() &&
//       player->box.getCenter().y > worldBox.getTop()) {
//     return false;
//   }
//
//   return true;
// }
//
// auto LDTK::getLevelJson(std::string iid) -> json {
//   for (auto [worldName, levels] : worlds) {
//     auto it = levels.find(iid);
//     if (it != levels.end()) {
//       json level = it->second;
//       return level;
//     }
//   }
//
//   std::cout << "Level Not Found" << "\n";
//   return {};
// }
//
// void LDTK::loadLevel(std::string iid, bool handleUnload) {
//   if (iid == "")
//     return;
//
//   // GameManager::updateEntities = false;
//   std::vector<Entity *> lastEntities = entities;
//
//   currentLevel = getLevelJson(iid);
//   auto &layerInstances = currentLevel["layerInstances"];
//
//   worldBox = {currentLevel["worldX"], currentLevel["worldY"],
//               currentLevel["pxWid"], currentLevel["pxHei"]};
//
//   for (auto it = layerInstances.rbegin(); it != layerInstances.rend(); ++it)
//   {
//     auto const &layer = *it;
//
//     if (layer["__type"] == "Entities") {
//       for (auto const &entity : layer["entityInstances"]) {
//         Entity *object = GameManager::createEntity(entity["__identifier"]);
//
//         object->add<Sprite>();
//         object->add<LDTKEntity>();
//
//         object->get<LDTKEntity>()->entityJson = entity;
//
//         object->box.position = {entity["px"][0], entity["px"][1]};
//         object->box.position += (worldBox.position);
//
//         object->box.size = {entity["width"], entity["height"]};
//
//         entities.push_back(object);
//       }
//     }
//   }
//
//   onLoadLevel(entities);
//
//   if (handleUnload) {
//     unload(lastEntities);
//     Camera::cameraLimitBox = worldBox;
//     Camera::setPosition(worldBox.getCenter());
//   } else {
//
//     Camera::cameraLimitBox = worldBox;
//   }
// }
//
// void LDTK::loadJson(std::string file) {
//   std::ifstream f(file);
//   fullJSON = json::parse(f);
//   jsonDir = std::filesystem::path(file).parent_path();
//
//   auto flags = fullJSON["flags"];
//   bool isMultiworld =
//       std::find(flags.begin(), flags.end(), "MultiWorlds") != flags.end();
//   if (isMultiworld) {
//     for (json world : fullJSON["worlds"]) {
//       for (json level : world["levels"]) {
//         worlds[world["identifier"]][level["iid"]] = level;
//       }
//     }
//
//   } else {
//     for (json level : fullJSON["levels"]) {
//       worlds["World"][level["iid"]] = level;
//     }
//   }
// }
