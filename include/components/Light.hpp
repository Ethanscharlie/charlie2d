#pragma once
#include "GameManager.hpp"
#include "Image.hpp"
#include "Math.hpp"
#include "ResourceManager.hpp"
#include "SDL_render.h"
#include "ShadowFilter.hpp"

class Light : public Component {
public:
  Light(Entity &entity) : Component(entity) {}
  void update() override;

  Image image;
  Uint8 r = 255;
  Uint8 g = 255;
  Uint8 b = 255;
};
