#pragma once
#include "Entity.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "GameManager.h"
#include "ResourceManager.h"

class UISliceRenderer : public Component {
    public:
    UISliceRenderer() : Component("UISliceRenderer") {};

    void setColor(std::array<Uint8, 3> color) {
        texture = ResourceManager::getInstance(GameManager::renderer).getColoredTexture(color, textureName);
    }

    void start() override {
        loadTexture(textureName);
        layer = 65;
        useLayer = true;
    }

    void loadTexture(std::string name) {
        textureName = name;
        texture = ResourceManager::getInstance(GameManager::renderer).getTexture(textureName);
    }

    void update(float deltaTime) override {
        SDL_Rect spriteRect = {0,0,0,0};
        
        float width_scale =  (float) GameManager::currentWindowSize.x  / GameManager::gameWindowSize.x;
        float height_scale = (float) GameManager::currentWindowSize.y / GameManager::gameWindowSize.y;

        if (true)
        {
            spriteRect.x = GameManager::screen_change_scale * (box->getPosition().x) + GameManager::currentWindowSize.x/2; 
            spriteRect.y = GameManager::screen_change_scale * (box->getPosition().y) + GameManager::currentWindowSize.y/2;    

            spriteRect.w = box->getSize().x * GameManager::screen_change_scale;
            spriteRect.h = box->getSize().y * GameManager::screen_change_scale;
        }
        else {
            spriteRect.x = GameManager::screen_change_scale * (box->getPosition().x) ;//+ GameManager::windowWidth/2; 
            spriteRect.y = GameManager::screen_change_scale * (box->getPosition().y) ;//+ GameManager::windowHeight/2;    

            spriteRect.w = box->getSize().x * GameManager::screen_change_scale;
            spriteRect.h = box->getSize().y * GameManager::screen_change_scale;
        }
        
        Draw9SlicedTexture(GameManager::renderer, texture, spriteRect, 10);
    }

    void Draw9SlicedTexture(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect destRect, int borderSize)
    {
        int textureWidth, textureHeight;
        SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);

        // Define the nine source rectangles
        SDL_Rect srcRect[9] = {
            // Top-left corner
            { 0, 0, borderSize, borderSize },
            // Top edge
            { borderSize, 0, textureWidth - (2 * borderSize), borderSize },
            // Top-right corner
            { textureWidth - borderSize, 0, borderSize, borderSize },
            // Left edge
            { 0, borderSize, borderSize, textureHeight - (2 * borderSize) },
            // Center
            { borderSize, borderSize, textureWidth - (2 * borderSize), textureHeight - (2 * borderSize) },
            // Right edge
            { textureWidth - borderSize, borderSize, borderSize, textureHeight - (2 * borderSize) },
            // Bottom-left corner
            { 0, textureHeight - borderSize, borderSize, borderSize },
            // Bottom edge
            { borderSize, textureHeight - borderSize, textureWidth - (2 * borderSize), borderSize },
            // Bottom-right corner
            { textureWidth - borderSize, textureHeight - borderSize, borderSize, borderSize }
        };

        // Define the nine destination rectangles
        SDL_Rect dstRect[9] = {
            // Top-left corner
            { destRect.x, destRect.y, borderSize, borderSize },
            // Top edge
            { destRect.x + borderSize, destRect.y, destRect.w - (2 * borderSize), borderSize },
            // Top-right corner
            { destRect.x + destRect.w - borderSize, destRect.y, borderSize, borderSize },
            // Left edge
            { destRect.x, destRect.y + borderSize, borderSize, destRect.h - (2 * borderSize) },
            // Center
            { destRect.x + borderSize, destRect.y + borderSize, destRect.w - (2 * borderSize), destRect.h - (2 * borderSize) },
            // Right edge
            { destRect.x + destRect.w - borderSize, destRect.y + borderSize, borderSize, destRect.h - (2 * borderSize) },
            // Bottom-left corner
            { destRect.x, destRect.y + destRect.h - borderSize, borderSize, borderSize },
            // Bottom edge
            { destRect.x + borderSize, destRect.y + destRect.h - borderSize, destRect.w - (2 * borderSize), borderSize },
            // Bottom-right corner
            { destRect.x + destRect.w - borderSize, destRect.y + destRect.h - borderSize, borderSize, borderSize }
        };

        // Draw the nine slices of the texture
        for (int i = 0; i < 9; i++) {
            SDL_RenderCopy(renderer, texture, &srcRect[i], &dstRect[i]);
        }
    }

    private:
    SDL_Texture* texture;
    std::string textureName = "img/panel.png";
};
