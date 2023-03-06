#pragma once

#include <iostream>
#include <SDL.h>
#include <vector>
#include <SDL_ttf.h>

#include "ResourceManager.h"
#include "GameObject.h"
#include "Scene.h"

class UIElement : public GameObject
{
    public:
    UIElement(SDL_Renderer* rendererIn, Scene* sceneIn, const SDL_Rect& rect);
    virtual ~UIElement();
    // virtual void update(float deltaTime) override = 0;
    void draw() override;
    void Draw9SlicedTexture(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect destRect, int borderSize);
    void renderTextInRect(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, const SDL_Rect& rect);    
     std::string text;

    protected:
    SDL_Rect UIRect;
    TTF_Font* font;
    SDL_Surface* textSurface;
    SDL_Color textColor;
    SDL_Texture* textTexture;
};