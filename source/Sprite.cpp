#include "Sprite.h"

void Sprite::update(float deltaTime) {
    SDL_Rect renderRect;
    Vector2f renderPos = object->box->getPosition() * SceneManager::screen_change_scale;
    renderRect.x = renderPos.x - (SceneManager::screen_change_scale * SceneManager::camera.x - SceneManager::windowWidth  / 2); 
    renderRect.y = renderPos.y - (SceneManager::screen_change_scale * SceneManager::camera.y - SceneManager::windowHeight / 2);   

    renderRect.w = object->box->getSize().x * SceneManager::screen_change_scale;
    renderRect.h = object->box->getSize().y * SceneManager::screen_change_scale;

    SDL_Rect* srcrect;
    if (sourceRect.w == 0 && sourceRect.h == 0) {
        srcrect = nullptr;
    } else {
        srcrect = &sourceRect;
    }

    if (texture != nullptr) {
        SDL_RenderCopyEx(SceneManager::renderer, texture, srcrect, &renderRect, (180 / M_PI) * std::atan2(angle.vector.y, angle.vector.x), nullptr, SDL_FLIP_NONE);
    }

    if (showBorders) {
        SDL_SetRenderDrawColor(SceneManager::renderer, 0, 255, 0, 255);
        SDL_RenderDrawRect(SceneManager::renderer, &renderRect);  
    }

    for (std::pair<std::string, Animation*> animation : animations) {
        animation.second->update();
    }

    //if (object->checkComponent<Collider>()) {
    //    Vector2f renderPos = object->getComponent<Collider>().colliderBox.position * SceneManager::screen_change_scale;
    //    renderRect.x = renderPos.x - (SceneManager::screen_change_scale * SceneManager::camera.x - SceneManager::windowWidth  / 2); 
    //    renderRect.y = renderPos.y - (SceneManager::screen_change_scale * SceneManager::camera.y - SceneManager::windowHeight / 2);   

    //    renderRect.w = object->getComponent<Collider>().colliderBox.size.x * SceneManager::screen_change_scale;
    //    renderRect.h = object->getComponent<Collider>().colliderBox.size.y * SceneManager::screen_change_scale;

    //    SDL_SetRenderDrawColor(SceneManager::renderer, 0, 255, 0, 255);
    //    SDL_RenderDrawRect(SceneManager::renderer, &renderRect);  
    //}
}

void Sprite::loadTexture(const std::string& image, bool setSize, bool keepCentered)
{
    texture = ResourceManager::getInstance(SceneManager::renderer).getTexture(image);
    textureName = image;

    SDL_Rect spriteRect;
    SDL_QueryTexture(texture, nullptr, nullptr, &spriteRect.w, &spriteRect.h);

    if (setSize) {
        if (keepCentered) {
            object->box->setScale({spriteRect.w, spriteRect.h});
        } else {
            object->box->setSize({spriteRect.w, spriteRect.h});
        }
    }
}

void Sprite::addAnimation(std::string title, std::vector<std::string> frames, float fps)
{
    animations[title] = new Animation(this, frames, fps);
}
