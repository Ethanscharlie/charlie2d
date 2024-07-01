#include "Sprite.h"
#include "Camera.h"
#include "Entity.h"
#include "Math.h"
#include "SDL_render.h"
#include <string>

void Sprite::update(float deltaTime) {
  Box renderBox = getRenderBox(entity);

  image.render(renderBox, angle);

  if (showBorders) {
    SDL_SetRenderDrawColor(GameManager::renderer, 0, 255, 0, 255);
    SDL_Rect renderRect = renderBox;
    SDL_RenderDrawRect(GameManager::renderer, &renderRect);
  }

  for (std::pair<std::string, Animation *> animation : animations) {
    animation.second->update();
  }
}

void Sprite::setAlpha(Uint8 alpha) { this->image.alpha = alpha; }

void Sprite::loadTexture(const std::string &image, bool setSize,
                         bool keepCentered) {
  this->image = Image(image);

  SDL_Rect spriteRect;
  SDL_QueryTexture(this->image.texture, nullptr, nullptr, &spriteRect.w,
                   &spriteRect.h);

  if (setSize) {
    if (keepCentered) {
      entity->box.setScale(
          {static_cast<float>(spriteRect.w), static_cast<float>(spriteRect.h)});
    } else {
      entity->box.size = {static_cast<float>(spriteRect.w),
                          static_cast<float>(spriteRect.h)};
    }
  }
}

void Sprite::addAnimation(std::string title, std::vector<std::string> frames,
                          float fps) {
  animations[title] = new Animation(this, frames, fps);
}
