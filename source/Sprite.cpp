#include "Sprite.h"
#include "Entity.h"
#include "Camera.h"

void Sprite::update(float deltaTime) {
    SDL_Rect renderRect;

    Vector2f renderPos = entity->box->getPosition() - Camera::getPosition();
    renderPos = renderPos * Camera::getScale();
    renderPos += GameManager::gameWindowSize/2;
    renderRect.x = renderPos.x;
    renderRect.y = renderPos.y;

    renderRect.w = entity->box->getSize().x * Camera::getScale() + 1;
    renderRect.h = entity->box->getSize().y * Camera::getScale() + 1;

    if (renderAsUI) {
        renderRect.x = entity->box->getPosition().x + GameManager::gameWindowSize.x/2;
        renderRect.y = entity->box->getPosition().y + GameManager::gameWindowSize.y/2;

        renderRect.w = entity->box->getSize().x;
        renderRect.h = entity->box->getSize().y;
    }


    SDL_Rect* srcrect;
    if (sourceRect.w == 0 && sourceRect.h == 0) {
        srcrect = nullptr;
    } else {
        srcrect = &sourceRect;
    }

        //onScreen = true;
    if (texture != nullptr &&
            renderRect.x+renderRect.w/1*5 > 0-renderRect.w/1 && 
            renderRect.x+renderRect.w/1 < GameManager::gameWindowSize.x+renderRect.w/1*5 && 
            renderRect.y+renderRect.h/1*5 > 0-renderRect.h/1 
            && renderRect.y+renderRect.h/1 < GameManager::gameWindowSize.y+renderRect.h/1*5) {

        onScreen = true;
        SDL_RenderCopyEx(GameManager::renderer, texture, srcrect, &renderRect, (180 / M_PI) * angle.radians, nullptr, flip);
//std::atan2(angle.vector.y, angle.vector.x)
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
