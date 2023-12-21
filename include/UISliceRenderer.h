#pragma once
#include "Camera.h"
#include "Entity.h"
#include "EntityBox.h"
#include "GameManager.h"
#include "ResourceManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

/**
 * \brief Renders a texture without stretching the corners
 * Don't forget to use a UICanvas
 */
class UISliceRenderer : public Component {
public:
  UISliceRenderer() : Component("UISliceRenderer"){};

  void setColor(std::array<Uint8, 3> color) {
    texture = ResourceManager::getInstance(GameManager::renderer)
                  .getColoredTexture(color, textureName);
  }

  void start() override {
    loadTexture(textureName);
    entity->layer = 65;
    entity->useLayer = true;
  }

  /**
   * \brief Loads a texture from filepath, this is meant to be square for a
   * panel of some sort
   */
  void loadTexture(std::string name) {
    textureName = name;
    texture = ResourceManager::getInstance(GameManager::renderer)
                  .getTexture(textureName);
  }

  void update(float deltaTime) override {
    SDL_Rect spriteRect = {0, 0, 0, 0};

    // float scaler = GameManager::screen_change_scale * Camera::getScale();
    Vector2f renderPos = entity->require<entityBox>()->getPosition() + GameManager::gameWindowSize/2;
    spriteRect.x = renderPos.x; //+ GameManager::camera.getCenter().x;
    spriteRect.y = renderPos.y; //+ GameManager::camera.getCenter().y;

    spriteRect.w = entity->require<entityBox>()->getSize().x; 
    spriteRect.h = entity->require<entityBox>()->getSize().y; 

    Draw9SlicedTexture(GameManager::renderer, texture, spriteRect, 10);
  }

  void Draw9SlicedTexture(SDL_Renderer *renderer, SDL_Texture *texture,
                          SDL_Rect destRect, int borderSize) {
    int textureWidth, textureHeight;
    SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);

    // Define the nine source rectangles
    SDL_Rect srcRect[9] = {
        // Top-left corner
        {0, 0, borderSize, borderSize},
        // Top edge
        {borderSize, 0, textureWidth - (2 * borderSize), borderSize},
        // Top-right corner
        {textureWidth - borderSize, 0, borderSize, borderSize},
        // Left edge
        {0, borderSize, borderSize, textureHeight - (2 * borderSize)},
        // Center
        {borderSize, borderSize, textureWidth - (2 * borderSize),
         textureHeight - (2 * borderSize)},
        // Right edge
        {textureWidth - borderSize, borderSize, borderSize,
         textureHeight - (2 * borderSize)},
        // Bottom-left corner
        {0, textureHeight - borderSize, borderSize, borderSize},
        // Bottom edge
        {borderSize, textureHeight - borderSize,
         textureWidth - (2 * borderSize), borderSize},
        // Bottom-right corner
        {textureWidth - borderSize, textureHeight - borderSize, borderSize,
         borderSize}};

    // Define the nine destination rectangles
    SDL_Rect dstRect[9] = {
        // Top-left corner
        {destRect.x, destRect.y, borderSize, borderSize},
        // Top edge
        {destRect.x + borderSize, destRect.y, destRect.w - (2 * borderSize),
         borderSize},
        // Top-right corner
        {destRect.x + destRect.w - borderSize, destRect.y, borderSize,
         borderSize},
        // Left edge
        {destRect.x, destRect.y + borderSize, borderSize,
         destRect.h - (2 * borderSize)},
        // Center
        {destRect.x + borderSize, destRect.y + borderSize,
         destRect.w - (2 * borderSize), destRect.h - (2 * borderSize)},
        // Right edge
        {destRect.x + destRect.w - borderSize, destRect.y + borderSize,
         borderSize, destRect.h - (2 * borderSize)},
        // Bottom-left corner
        {destRect.x, destRect.y + destRect.h - borderSize, borderSize,
         borderSize},
        // Bottom edge
        {destRect.x + borderSize, destRect.y + destRect.h - borderSize,
         destRect.w - (2 * borderSize), borderSize},
        // Bottom-right corner
        {destRect.x + destRect.w - borderSize,
         destRect.y + destRect.h - borderSize, borderSize, borderSize}};

    // Draw the nine slices of the texture
    for (int i = 0; i < 9; i++) {
      SDL_RenderCopy(renderer, texture, &srcRect[i], &dstRect[i]);
    }
  }

private:
  SDL_Texture *texture;
  std::string textureName = "img/panel.png";
};
