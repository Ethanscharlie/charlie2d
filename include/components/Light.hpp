#pragma once
#include "GameManager.hpp"
#include "Image.hpp"
#include "Math.hpp"
#include "ResourceManager.hpp"
#include "SDL_render.h"
#include "Serializer.hpp"
#include "ShadowFilter.hpp"

class Light : public Component {
public:
  Light() {
    typeIsRendering = true;
    propertyRegister = {GET_PROP(image), GET_PROP(r), GET_PROP(g), GET_PROP(b)};
  }

  void start() override {}

  void update(float deltaTime) override;

  Image image;
  Uint8 r = 255;
  Uint8 g = 255;
  Uint8 b = 255;
};
REGISTER_COMPONENT_TYPE(Light);
