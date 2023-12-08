#pragma once
#include "Entity.h"
#include "EntityBox.h"
#include "GameManager.h"
#include "ResourceManager.h"
#include "UISliceRenderer.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

/**
 * \brief Sets the box to be the size of the screen (For UI anchoring)
 * For UI please add a canvas entity and add this Component then use setParent
 * to set this as the parent
 */
class UICanvas : public Component {
public:
  UICanvas() : Component("UICanvas"){};

  void start() override {

    entity->require<entityBox>()->setSize(GameManager::gameWindowSize);

    // Margin
    // entity->require<entityBox>()->changeScale({-10, -10});

    entity->require<entityBox>()->setWithCenter({0, 0});
  }

  void update(float deltaTime) override {
    if (entity->toDestroy)
      return;
    start();
  }
};
