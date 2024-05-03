#pragma once
#include "Component.h"
#include "ExtendedComponent.h"
#include "Serializer.h"
#include "Vector2f.h"
#include <Charlie2D.h>

enum class AlignmentAnchor {
  topLeft,
  topMiddle,
  topRight,
  middleLeft,
  center,
  middleRight,
  bottomLeft,
  bottomMiddle,
  bottomRight
};

class Alignment : public Component {
public:
  Alignment() {
    propertyRegister = {GET_PROP(parent), GET_PROP(anchor), GET_PROP(offset)};
    typeIsRendering = true;
  }

  void start() override {}

  void update(float deltaTime) override {
    if (parent == nullptr)
      return;
    entity->box.setWithCenter(parent->box.getCenter() + offset);
  }

  AlignmentAnchor anchor;
  Vector2f offset;
  Entity *parent = nullptr;
};
REGISTER_COMPONENT_TYPE(Alignment);
