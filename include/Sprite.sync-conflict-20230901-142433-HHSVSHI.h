#pragma once 

#include <SDL.h>
#include "GameObject.h"
#include "Component.h"
#include "Math.h"
#include "SceneManager.h"

class Sprite : public Component {
    public:
    Sprite() : Component("sprite") {};

    void start() override {}

    void update(float deltaTime) override {
        SDL_Rect renderRect;
        Vector2f renderPos = object->box->absoluteBox.position * SceneManager::screen_change_scale;
        renderRect.x = renderPos.x - (SceneManager::screen_change_scale * SceneManager::camera.x - SceneManager::windowWidth  / 2); 
        renderRect.y = renderPos.y - (SceneManager::screen_change_scale * SceneManager::camera.y - SceneManager::windowHeight / 2);   

        renderRect.w = object->box->absoluteBox.size.x * SceneManager::screen_change_scale;
        renderRect.h = object->box->absoluteBox.size.y * SceneManager::screen_change_scale;

        SDL_Rect* srcrect;
        if (sourceRect.w == 0 && sourceRect.h == 0) {
            srcrect = nullptr;
        } else {
            srcrect = &sourceRect;
        }

        if (texture != nullptr) {
            SDL_RenderCopyEx(SceneManager::renderer, texture, srcrect, &renderRect, (180 / M_PI) * std::atan2(angle.vector.y, angle.vector.x), nullptr, SDL_FLIP_NONE);
        }

        if (showBorders) {
            SDL_SetRenderDrawColor(SceneManager::renderer, 0, 255, 0, 255);
            SDL_RenderDrawRect(SceneManager::renderer, &renderRect);  
        }
    }

    void loadTexture(const std::string& image, bool setSize=true)
    {
        texture = ResourceManager::getInstance(SceneManager::renderer).getTexture(image);
        textureName = image;

        SDL_Rect spriteRect;
        SDL_QueryTexture(texture, nullptr, nullptr, &spriteRect.w, &spriteRect.h);

        if (setSize) {
            object->box->setSize({spriteRect.w, spriteRect.h});
        }
    }

    SDL_Rect sourceRect = {0, 0, 0, 0};
    SDL_Texture* texture = nullptr;
    std::string textureName = "";
    bool showBorders = true;
    Angle angle;
};
