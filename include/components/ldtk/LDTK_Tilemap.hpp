#pragma once
#include "Box.hpp"
#include "Component.hpp"
#include "Image.hpp"
#include "Math.hpp"
#include "SDL_render.h"
#include "ldtk/LDTK_TileLayer.hpp"
#include "nlohmann/json.hpp"
#include <string>

using json = nlohmann::json;

namespace LDTK {
class Tilemap : public Component {
public:
  void update(float deltaTime) override {
    if (!layer)
      return;
    Image image = {layer->texture};
    image.render(getRenderBox(renderBox));
  }

  Box renderBox;
  bool solid = true;
  TileLayer *layer = nullptr;
};
} // namespace LDTK
