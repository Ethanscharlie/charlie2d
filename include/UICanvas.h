#pragma once
#include "Entity.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "GameManager.h"
#include "ResourceManager.h"
#include "EntityBox.h"

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
