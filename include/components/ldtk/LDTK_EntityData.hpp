#pragma once
#include "Box.hpp"
#include "Component.hpp"
#include "Image.hpp"
#include "Math.hpp"
#include "SDL_render.h"
#include "ldtk/LDTK_EntityInstance.hpp"
#include "ldtk/LDTK_Level.hpp"
#include "ldtk/LDTK_Project.hpp"
#include "nlohmann/json.hpp"
#include <algorithm>
#include <string>

using json = nlohmann::json;

namespace LDTK {
class EntityData : public Component {
public:
  void onDestroy() override {
    instance->entity = nullptr;
    auto it = std::find(level->loadedEntites.begin(),
                        level->loadedEntites.end(), entity);
    if (it != level->loadedEntites.end()) {
      *it = nullptr;
    }
  }
  EntityInstance *instance = nullptr;
  Level *level = nullptr;
};
} // namespace LDTK
