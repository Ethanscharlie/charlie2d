#pragma once

#include "Camera.hpp"
#include "LDTKEntity.hpp"
#include "Tile.hpp"

#include <filesystem>
#include <iostream>
#include <vector>

#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

/**
 * \brief A loader for the Level Design Toolkit
 * The entity tag is set to the name of the entity within ldtk
 * Tiles are put of TileLayer s (But will also be tag named the same way)
 */
class LDTK {
public:
  LDTK();

  static void update(float deltaTime);
  /**
   * \brief Removes all entites created by ldtk
   * \param enlist Defaults to ldtk entities but can be any list of entities
   */
  static void unload(std::vector<Entity *> enlist = entities);
  /**
   * \brief Where did the player go?
   *
   * Made to be used with checkOutsideBounds
   * \param player The player Entity
   */
  static std::string findTraveledLevel(Entity *player);
  /**
   * \brief Checks to see in the player is outside the bounds of the level
   *
   * Made to be used with findTraveledLevel
   */
  static bool checkOutsideBounds(Entity *player);
  /**
   * \brief Loads a level via iid
   * \param iid The iid of the ldtk level
   * \param scene the current scene
   */
  static void loadLevel(std::string iid, bool handleUnload = true);
  /**
   * \brief Loads up the json for your world (Please do before anything else
   * here) \param file The filepath
   */
  static void loadJson(std::string file = "img/ldtk/space.ldtk");

  static json getLevelJson(std::string iid);

  static Box worldBox;
  static Entity *ldtkPlayer;

  /**
   * \brief List of all current entities loaded by ldtk
   */
  static std::vector<Entity *> entities;

  static json fullJSON;
  static json currentLevel;
  static std::map<std::string, std::map<std::string, json>> worlds;

  /**
   * \brief Runs when any level has finished loading
   *
   * Here you can loop through the entities std::vector and check the propertys
   * The entity tag is set to the name of the entity within ldtk, so you can
   * check for it in the loop and add components to different entity types
   */
  static std::function<void(std::vector<Entity*>)> onLoadLevel;

  static void preload(std::string iid);

  static std::map<std::string, std::map<std::string, TileLayer>>
      preloadedTiles;

private:
  static std::filesystem::path jsonDir;
};
