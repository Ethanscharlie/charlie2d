#pragma once
#include "GameManager.h"
#include "Math.h"
#include "ResourceManager.h"
#include "SDL_render.h"
#include "Serializer.h"
#include "ShadowFilter.h"

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
