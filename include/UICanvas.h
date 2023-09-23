#pragma once
#include "Entity.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "SceneManager.h"
#include "ResourceManager.h"

class UICanvas : public Component {
    public:
    UICanvas() : Component("UICanvas") {};

    void start() override {

        //float scaler = (float) SceneManager::windowWidth / SceneManager::originalWidth / SceneManager::screen_change_scale;
        //box->setSize({SceneManager::originalWidth  * scaler, SceneManager::originalHeight * scaler});
        box->setSize({SceneManager::windowWidth/ SceneManager::screen_change_scale, SceneManager::windowHeight / SceneManager::screen_change_scale});

        // Margin
        //box->changeSize({-10, -10});

        box->setWithCenter({0,0});
    }

    void update(float deltaTime) override {
        start();
    }
};
