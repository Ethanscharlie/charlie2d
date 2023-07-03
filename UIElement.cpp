#include <iostream>
#include <cmath>
#include <vector>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "ResourceManager.h"
#include "UIElement.h"
#include "Math.h"

UIElement::UIElement(const SDL_Rect& rect, const int _fontSize) : original_font_size(_fontSize), GameObject()
{   
    spriteRect = rect;
    size.x = rect.w;
    size.y = rect.h;

    text = "";
    changeFont(original_font_size);
    
    //moveTo({spriteRect.x, spriteRect.y});
    moveTo({static_cast<float>(spriteRect.x), static_cast<float>(spriteRect.y)});


    //originalSize = {spriteRect.w, spriteRect.h};
}

void UIElement::start() {}

void UIElement::startBase()
{
}

void UIElement::loadTexture(const std::string& image){
    texture = ResourceManager::getInstance(SceneManager::renderer).getTexture(image);        
    SDL_QueryTexture(texture, nullptr, nullptr, nullptr, nullptr);
}

UIElement::~UIElement()
{
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
}

void UIElement::update(float deltaTime)
{
    
}

void UIElement::updateBase(float deltaTime)
{
    GameObject::updateBase(deltaTime);

    SDL_Point mousePos;
    SDL_GetMouseState(&mousePos.x, &mousePos.y);
    if (SDL_PointInRect(&mousePos, &spriteRect))
    {
        onHover();
        const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
        if (InputManager::checkInput("fire"))
        {
            onClick();
        }
    }
    else
    {
        onNotHover();
    }
}

void UIElement::onHover()
{

}
void UIElement::onNotHover()
{

}

void UIElement::onClick()
{

}

void UIElement::draw() 
{
    //spriteRect.x = getPosition().x - centerOffset.x;
    //spriteRect.y = getPosition().y - centerOffset.y;
    //spriteRect.w = size.x;
    //spriteRect.h = size.y;

    float width_scale =  (float) SceneManager::windowWidth  / SceneManager::originalWidth;
    float height_scale = (float) SceneManager::windowHeight / SceneManager::originalHeight;

    //spriteRect.x = SceneManager::screen_change_scale * (getPosition().x - centerOffset.x) ;//+ SceneManager::windowWidth/2; 
    //spriteRect.y = SceneManager::screen_change_scale * (getPosition().y - centerOffset.y) ;//+ SceneManager::windowHeight/2;    

    //spriteRect.w = size.x * SceneManager::screen_change_scale;
    //spriteRect.h = size.y * SceneManager::screen_change_scale;
    
    if (ignore_screen_resize || true)
    {
        spriteRect.x = SceneManager::screen_change_scale * (getPosition().x - centerOffset.x) + SceneManager::windowWidth/2; 
        spriteRect.y = SceneManager::screen_change_scale * (getPosition().y - centerOffset.y) + SceneManager::windowHeight/2;    

        spriteRect.w = size.x * SceneManager::screen_change_scale;
        spriteRect.h = size.y * SceneManager::screen_change_scale;
    }
    else {
        spriteRect.x = SceneManager::screen_change_scale * (getPosition().x - centerOffset.x) ;//+ SceneManager::windowWidth/2; 
        spriteRect.y = SceneManager::screen_change_scale * (getPosition().y - centerOffset.y) ;//+ SceneManager::windowHeight/2;    

        spriteRect.w = size.x * SceneManager::screen_change_scale;
        spriteRect.h = size.y * SceneManager::screen_change_scale;
    }

    Draw9SlicedTexture(SceneManager::renderer, texture, spriteRect, 10);
    if (text.length() > 0)
    {
        renderTextInRect(SceneManager::renderer, text, spriteRect);
    }
}

void UIElement::Draw9SlicedTexture(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect destRect, int borderSize)
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

void UIElement::changeFont(int size)
{
    fontSize = size;
    font = TTF_OpenFont("img/fonts/Silkscreen-Regular.ttf", fontSize);
}

void UIElement::renderTextInRect(SDL_Renderer* renderer, const std::string& text, const SDL_Rect& rect)
{
    // Create a surface from the text
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

