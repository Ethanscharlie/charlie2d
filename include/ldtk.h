///#pragma once
///
///#include "Charlie2D.h"
///#include "LDTKEntity.h"
///#include <iostream>
///#include <vector>
///#include <fstream>
///#include <nlohmann/json.hpp>
///using json = nlohmann::json;
///
///
///class LDTK {
///    public:
///    LDTK();
///
///    static void update(float deltaTime);
///    static void unload(std::vector<Entity*> enlist);
///    static std::string findTraveledLevel(Entity* player);
///    static bool checkOutsideBounds(Entity* player);
///    static void loadLevel(std::string iid, Scene* scene);
///    static void loadJson(std::string file="img/ldtk/icnt.ldtk");
///
///    static Box worldBox;
///    static Entity* ldtkPlayer;
///    static std::vector<Entity*> entities;
///
///    static json fullJSON;
///    static json currentLevel;
///    static std::map<std::string, json> levels;
///
///    static std::function<void()> onLoadLevel;
///};
///
