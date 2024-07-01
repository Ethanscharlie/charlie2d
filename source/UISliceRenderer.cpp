#include "UISliceRenderer.h"

void UISliceRenderer::start() {
  entity->layer = 65;
  entity->useLayer = true;
}

/**
 * \brief Loads a texture from filepath, this is meant to be square for a
 * panel of some sort
 */
void UISliceRenderer::update(float deltaTime) {
  SDL_Rect spriteRect = {0, 0, 0, 0};

  if (!rendererInWorld) {
    Vector2f renderPos = entity->box.position + GameManager::gameWindowSize / 2;
    spriteRect.x = renderPos.x; //+ GameManager::camera.getCenter().x;
    spriteRect.y = renderPos.y; //+ GameManager::camera.getCenter().y;
    spriteRect.w = entity->box.size.x;
    spriteRect.h = entity->box.size.y;

  } else {
    Vector2f renderPos = entity->box.position - Camera::getPosition();
    renderPos = renderPos * Camera::getScale();
    renderPos += GameManager::gameWindowSize / 2;
    spriteRect.x = renderPos.x;
    spriteRect.y = renderPos.y;

    spriteRect.w = entity->box.size.x * Camera::getScale();
    spriteRect.h = entity->box.size.y * Camera::getScale();
  }

  SDL_SetTextureBlendMode(image.texture, image.blendMode);
  SDL_SetTextureAlphaMod(image.texture, image.alpha);

  Draw9SlicedTexture(GameManager::renderer, image.texture, spriteRect, 10);
}

void UISliceRenderer::Draw9SlicedTexture(SDL_Renderer *renderer,
                                         SDL_Texture *texture,
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
      {borderSize, textureHeight - borderSize, textureWidth - (2 * borderSize),
       borderSize},
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
