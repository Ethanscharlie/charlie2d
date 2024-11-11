#pragma once
#include "Component.hpp"
#include "Entity.hpp"
#include "Vector2f.hpp"

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
  Alignment(Entity &entity) : Component(entity) {}

  void update() override {
    if (parent == nullptr)
      return;
    entity.box.setWithCenter(parent->box.getCenter() + offset);
  }

  AlignmentAnchor anchor;
  Vector2f offset;
  Entity *parent = nullptr;
};
