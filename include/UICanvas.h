#pragma once
#include "Entity.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "GameManager.h"
#include "ResourceManager.h"
#include "EntityBox.h"

/**
 * \brief Sets the box to be the size of the screen (For UI anchoring)
 * For UI please add a canvas entity and add this Component then use setParent to set this as the parent
 */
class UICanvas : public Component {
    public:
    UICanvas() : Component("UICanvas") {};

    void start() override {

        //float scaler = (float) GameManager::windowWidth / GameManager::originalWidth / GameManager::screen_change_scale;
        //box->setSize({GameManager::originalWidth  * scaler, GameManager::originalHeight * scaler});
        box->setSize({GameManager::currentWindowSize.x/ GameManager::screen_change_scale, GameManager::currentWindowSize.y / GameManager::screen_change_scale});

        // Margin
        //box->changeSize({-10, -10});

        box->setWithCenter({0,0});
    }

    void update(float deltaTime) override {
        start();
    }
};
