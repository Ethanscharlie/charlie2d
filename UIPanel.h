#pragma once

#include <iostream>
#include <SDL.h>
#include <vector>
#include <initializer_list>
#include <array>
#include <SDL_ttf.h>

#include "ResourceManager.h"
#include "SceneManager.h"
#include "UIElement.h"
#include "Scene.h"

class UIPanel : public UIElement
{
    public:
    UIPanel(const SDL_Rect& rect, const std::string& imageIn);
    virtual ~UIPanel();
    void startBase() override;
    void update(float deltaTime) override;

    private:
    std::string image;
    SDL_Texture* mainTexture;
};
