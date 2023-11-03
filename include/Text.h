#pragma once
#include "Entity.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "GameManager.h"
#include "ResourceManager.h"

class Text : public Component {
    public:
    Text() : Component("Text") {};

    void start() override {
        text = "";
        fontSize = original_font_size;
        changeFont(fontFile, original_font_size);
        layer = 70;
        useLayer = true;
    }

    void update(float deltaTime) override {
        if (text.length() <= 0) return;
        //if (fontFile == "") return;
        if (fontSize == 0) return;

        SDL_Rect spriteRect = {0,0,0,0};
        
        float width_scale =  (float) GameManager::currentWindowSize.x / GameManager::gameWindowSize.x;
        float height_scale = (float) GameManager::currentWindowSize.y / GameManager::gameWindowSize.y;

        //spriteRect.x = GameManager::screen_change_scale * (box->getPosition().x) + GameManager::currentWindowSize.x/2; 
        //spriteRect.y = GameManager::screen_change_scale * (box->getPosition().y) + GameManager::currentWindowSize.y/2;    
            Vector2f renderPos = entity->box->getUIPosition();
            spriteRect.x = renderPos.x;//+ GameManager::camera.getCenter().x;
            spriteRect.y = renderPos.y;//+ GameManager::camera.getCenter().y;

        float scaler = GameManager::screen_change_scale * ((GameManager::gameWindowSize.x + GameManager::gameWindowSize.y) / (GameManager::camera.size.x + GameManager::camera.size.y));
        spriteRect.w = box->getSize().x * GameManager::screen_change_scale;
        spriteRect.h = box->getSize().y * GameManager::screen_change_scale;

        renderTextInRect(GameManager::renderer, text, spriteRect);
    }

    void changeFont(std::string fontFile, int size)
    {
        fontSize = size;
        font = TTF_OpenFont(fontFile.c_str(), fontSize * GameManager::screen_change_scale);
    }

    void renderTextInRect(SDL_Renderer* renderer, const std::string& text, const SDL_Rect& rect)
    {
        // Create a surface from the text
        TTF_SetFontWrappedAlign(font, TTF_WRAPPED_ALIGN_CENTER);
        SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), text_color, rect.w);

        if (textSurface == nullptr) {
            std::cerr << "Error creating surface: " << SDL_GetError() << std::endl;
            return;
        }

        // Create a texture from the surface
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        if (textTexture == nullptr) {
            std::cerr << "Error creating texture: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(textSurface);
            return;
        }

        // Calculate the destination rectangle to center the text
        SDL_Rect dstRect;
        dstRect.w = textSurface->w;
        dstRect.h = textSurface->h;
        dstRect.x = rect.x + (rect.w - textSurface->w) / 2;
        dstRect.y = rect.y + (rect.h - textSurface->h) / 2;

        // Render the texture to the renderer
        SDL_RenderCopy(renderer, textTexture, nullptr, &dstRect);

        // Clean up resources
        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);
    }

    Box textBox;

    std::string text;

    SDL_Color text_color = {0, 0, 0};
    int original_font_size = 20;
    int fontSize;
    std::string fontFile = "";
    TTF_Font* font = TTF_OpenFont(fontFile.c_str(), fontSize);
    SDL_Color textColor;
};
