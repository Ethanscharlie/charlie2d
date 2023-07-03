#pragma once

#include <iostream>
#include <SDL.h>
#include <vector>
#include <array>
#include <SDL_ttf.h>

#include "ResourceManager.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Scene.h"

class UIElement : public GameObject
{
    public:
    UIElement(const SDL_Rect& rect, const int _fontSize=18);
    virtual ~UIElement();
    virtual void start() override;
    virtual void startBase() override;
    void updateBase(float deltaTime) override;
    virtual void update(float deltaTime) override;
    void loadTexture(const std::string& image) override;
    void draw() override;
    void Draw9SlicedTexture(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect destRect, int borderSize);
    void changeFont(int size);
    void renderTextInRect(SDL_Renderer* renderer, const std::string& text, const SDL_Rect& rect); 
    virtual void onHover();
    virtual void onNotHover();
    virtual void onClick();

    std::string text;

    protected:
    SDL_Color text_color = {0, 0, 0};
    int original_font_size = 1;
    int fontSize = 1;
    bool ignore_screen_resize = false;
    TTF_Font* font = TTF_OpenFont("img/fonts/Silkscreen-Regular.ttf", fontSize);
    SDL_Surface* textSurface;
    SDL_Color textColor;
    SDL_Texture* textTexture;
};
