#pragma once

#include <iostream>
#include <SDL.h>
#include <vector>
#include <SDL_ttf.h>

#include "ResourceManager.h"
#include "SceneManager.h"
#include "UIElement.h"
#include "Scene.h"

class UICanvas : public UIElement
{
    public:
    UICanvas();
    virtual ~UICanvas();
    void startBase() override;
    void update(float deltaTime) override;
    
    //SDL_Texture* base_texture;
    //SDL_Texture* selected_texture;
};
