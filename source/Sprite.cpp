#include "Sprite.h"
#include "Camera.h"
#include "Entity.h"
#include "SDL_render.h"
#include <string>

void Sprite::update(float deltaTime) {
  SDL_Rect renderRect;

  Vector2f renderPos =
      entity->require<entityBox>()->getPosition() - Camera::getPosition();
  renderPos = renderPos * Camera::getScale();
  renderPos += GameManager::gameWindowSize / 2;
  renderRect.x = renderPos.x;
  renderRect.y = renderPos.y;

  renderRect.w =
      entity->require<entityBox>()->getSize().x * Camera::getScale() +
      preventWeirdBorder;
  renderRect.h =
      entity->require<entityBox>()->getSize().y * Camera::getScale() +
      preventWeirdBorder;

  if (renderAsUI) {
    renderRect.x = entity->require<entityBox>()->getPosition().x +
                   GameManager::gameWindowSize.x / 2;
    renderRect.y = entity->require<entityBox>()->getPosition().y +
                   GameManager::gameWindowSize.y / 2;

    renderRect.w = entity->require<entityBox>()->getSize().x;
    renderRect.h = entity->require<entityBox>()->getSize().y;
  }

  SDL_Rect *srcrect;
  if (sourceRect.w == 0 && sourceRect.h == 0) {
    srcrect = nullptr;
  } else {
    srcrect = &sourceRect;
  }

  SDL_SetTextureBlendMode(this->image.texture, SDL_BLENDMODE_BLEND);
  SDL_SetTextureAlphaMod(this->image.texture, alpha);

  if (this->image.texture != nullptr &&
      renderRect.x + renderRect.w / 1 * 5 > 0 - renderRect.w / 1 &&
      renderRect.x + (float)renderRect.w / 1 <
          GameManager::gameWindowSize.x + (float)renderRect.w / 1 * 5 &&
      renderRect.y + renderRect.h / 1 * 5 > 0 - renderRect.h / 1 &&
      renderRect.y + (float)renderRect.h / 1 <
          GameManager::gameWindowSize.y + (float)renderRect.h / 1 * 5) {

    onScreen = true;
    SDL_RenderCopyEx(GameManager::renderer, this->image.texture, srcrect, &renderRect,
                     (180 / M_PI) * angle.radians, nullptr, flip);

    SDL_SetTextureAlphaMod(this->image.texture, 255);
  } else {
    onScreen = false;
  }

  if (showBorders) {
    SDL_SetRenderDrawColor(GameManager::renderer, 0, 255, 0, 255);
    SDL_RenderDrawRect(GameManager::renderer, &renderRect);
  }

  for (std::pair<std::string, Animation *> animation : animations) {
    animation.second->update();
  }
}

void Sprite::setAlpha(Uint8 alpha) { this->alpha = alpha; }

void Sprite::loadTexture(const std::string &image, bool setSize,
                         bool keepCentered) {
  this->image = Image(image);

  SDL_Rect spriteRect;
  SDL_QueryTexture(this->image.texture, nullptr, nullptr, &spriteRect.w,
                   &spriteRect.h);

  if (setSize) {
    if (keepCentered) {
      entity->require<entityBox>()->setScale(
          {static_cast<float>(spriteRect.w), static_cast<float>(spriteRect.h)});
    } else {
      entity->require<entityBox>()->setSize(
          {static_cast<float>(spriteRect.w), static_cast<float>(spriteRect.h)});
    }
  }
}

void Sprite::addAnimation(std::string title, std::vector<std::string> frames,
                          float fps) {
  animations[title] = new Animation(this, frames, fps);
}
