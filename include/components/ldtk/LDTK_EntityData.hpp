#pragma once
#include "Box.hpp"
#include "Component.hpp"
#include "Image.hpp"
#include "Math.hpp"
#include "SDL_render.h"
#include "ldtk/LDTK_EntityInstance.hpp"
#include "ldtk/LDTK_Level.hpp"
#include "nlohmann/json.hpp"
#include <string>

using json = nlohmann::json;

namespace LDTK {
class EntityData : public Component {
public:
  EntityInstance *instance = nullptr;
  Level *level = nullptr;
};
} // namespace LDTK
