#include <iostream>
#include <cmath>
#include <vector>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "ResourceManager.h"
#include "UIElement.h"
#include "Math.h"

UIElement::UIElement(SDL_Renderer* rendererIn, Scene* sceneIn, const SDL_Rect& rect) : GameObject(rendererIn, sceneIn)
{   
    UIRect = rect;
    text = "";
}

UIElement::~UIElement()
{
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
}

void UIElement::draw() 
{
    Draw9SlicedTexture(renderer, texture, UIRect, 10);
    renderTextInRect(renderer, font, text, UIRect);
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

void UIElement::renderTextInRect(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, const SDL_Rect& rect)
{
    // Create a surface from the text
    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), {0, 0, 0}, rect.w);

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
