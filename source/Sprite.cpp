#include "Sprite.h"
#include "Entity.h"

void Sprite::update(float deltaTime) {
    float scaler = GameManager::screen_change_scale * ((GameManager::gameWindowSize.x + GameManager::gameWindowSize.y) / (GameManager::camera.size.x + GameManager::camera.size.y));
    SDL_Rect renderRect;
    Vector2f renderPos = entity->box->getPosition() * scaler;
    renderRect.x = renderPos.x - (scaler * GameManager::camera.getCenter().x - GameManager::currentWindowSize.x  / 2); 
    renderRect.y = renderPos.y - (scaler * GameManager::camera.getCenter().y - GameManager::currentWindowSize.y / 2);   

    renderRect.w = entity->box->getSize().x * scaler + 1;
    renderRect.h = entity->box->getSize().y * scaler + 1;

    SDL_Rect* srcrect;
    if (sourceRect.w == 0 && sourceRect.h == 0) {
        srcrect = nullptr;
    } else {
        srcrect = &sourceRect;
    }

    if (texture != nullptr && renderRect.x+renderRect.w/2 > 0-renderRect.w/2 && renderRect.x+renderRect.w/2 < GameManager::currentWindowSize.x+renderRect.w/2 && 
            renderRect.y+renderRect.h/2 > 0-renderRect.h/2 && renderRect.y+renderRect.h/2 < GameManager::currentWindowSize.y+renderRect.h/2) {

        onScreen = true;
        SDL_RenderCopyEx(GameManager::renderer, texture, srcrect, &renderRect, (180 / M_PI) * std::atan2(angle.vector.y, angle.vector.x), nullptr, SDL_FLIP_NONE);
    } else {onScreen = false;}

    if (showBorders) {
        SDL_SetRenderDrawColor(GameManager::renderer, 0, 255, 0, 255);
        SDL_RenderDrawRect(GameManager::renderer, &renderRect);  
    }

    for (std::pair<std::string, Animation*> animation : animations) {
        animation.second->update();
    }

    //if (entity->checkComponent<Collider>()) {
    //    Vector2f renderPos = entity->getComponent<Collider>().colliderBox.position * GameManager::screen_change_scale;
    //    renderRect.x = renderPos.x - (GameManager::screen_change_scale * GameManager::camera.x - GameManager::currentWindowSize.x  / 2); 
    //    renderRect.y = renderPos.y - (GameManager::screen_change_scale * GameManager::camera.y - GameManager::currentWindowSize.y / 2);   

    //    renderRect.w = entity->getComponent<Collider>().colliderBox.size.x * GameManager::screen_change_scale;
    //    renderRect.h = entity->getComponent<Collider>().colliderBox.size.y * GameManager::screen_change_scale;

    //    SDL_SetRenderDrawColor(GameManager::renderer, 0, 255, 0, 255);
    //    SDL_RenderDrawRect(GameManager::renderer, &renderRect);  
    //}
}

void Sprite::loadTexture(const std::string& image, bool setSize, bool keepCentered)
{
    texture = ResourceManager::getInstance(GameManager::renderer).getTexture(image);
    textureName = image;

    SDL_Rect spriteRect;
    SDL_QueryTexture(texture, nullptr, nullptr, &spriteRect.w, &spriteRect.h);

    if (setSize) {
        if (keepCentered) {
            entity->box->setScale({static_cast<float>(spriteRect.w), static_cast<float>(spriteRect.h)});
        } else {
            entity->box->setSize({static_cast<float>(spriteRect.w), static_cast<float>(spriteRect.h)});
        }
    }
}

void Sprite::addAnimation(std::string title, std::vector<std::string> frames, float fps)
{
    animations[title] = new Animation(this, frames, fps);
}
