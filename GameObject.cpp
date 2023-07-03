#include "GameObject.h"
#include "Scene.h"
#include "ResourceManager.h"

#include <iostream>
#include <vector>
#include "Math.h"

GameObject::GameObject() {}

GameObject::~GameObject() {}

void GameObject::init(Scene* sceneIn, const std::string& groupIn, const int& idIn, GameObject* parentIn)
{
    scene = sceneIn;
    group = groupIn;
    id = idIn;
    parent = parentIn;

    updateRealPosition();
}

void GameObject::start() {}
void GameObject::startBase() {}
void GameObject::update(float deltaTime) {}

void GameObject::updateBase(float deltaTime)
{
    updateRealPosition();
    for (std::pair<std::string, Animation*> animation : animations)
    {
        animation.second->update();
    }
}

void GameObject::draw() 
{
    if (texture == nullptr) {return;}

    spriteRect.x = SceneManager::screen_change_scale * (realPosition.x - centerOffset.x); 
    spriteRect.y = SceneManager::screen_change_scale * (realPosition.y - centerOffset.y);   

    spriteRect.w = size.x * SceneManager::screen_change_scale;
    spriteRect.h = size.y * SceneManager::screen_change_scale;
        
    if (debug) {
        SDL_SetRenderDrawColor(SceneManager::renderer, 0, 255, 0, 255);
        SDL_RenderDrawRect(SceneManager::renderer, &spriteRect);  
    }

    SDL_RenderCopyEx(SceneManager::renderer, texture, nullptr, &spriteRect, (180 / M_PI) * std::atan2(direction.y, direction.x), nullptr, SDL_FLIP_NONE);
}

void GameObject::move(const Vector2f& change)
{
    position.x += change.x; 
    position.y += change.y;

    updateRealPosition();
}

void GameObject::moveTo(const Vector2f& change)
{
    position.x = change.x;
    position.y = change.y;

    updateRealPosition();
}

void GameObject::moveToX(float x) {
    position.x = x;
    updateRealPosition();
}

void GameObject::moveToY(float y) {
    position.y = y;
    updateRealPosition();
}

void GameObject::updateRealPosition()
{
    if (centered) {
        centerOffset.x = size.x / 2;
        centerOffset.y = size.y / 2;
    }

    if (parent == nullptr) {
        realPosition = position;
    }
    else
    {
        Vector2f parent_corner_pos = {parent->realPosition.x-parent->centerOffset.x, parent->realPosition.y-parent->centerOffset.y};

        if (anchor == 0) // Top Left
        {
            realPosition.x = position.x + parent_corner_pos.x;
            realPosition.y = position.y + parent_corner_pos.y;
        }
        else if (anchor == 1) // Top Middle
        {
            realPosition.x = position.x + (parent_corner_pos.x + parent->size.x/2);
            realPosition.y = position.y + parent_corner_pos.y;
        }
        else if (anchor == 2)
        {
            
        }
        else if (anchor == 3)
        {
            
        }
        else if (anchor == 4) // Middle
        {
            realPosition.x = position.x + (parent_corner_pos.x + parent->size.x/2);
            realPosition.y = position.y + (parent_corner_pos.y + parent->size.y/2);
        }
        else if (anchor == 5)
        {
            
        }
        else if (anchor == 6)
        {
            
        }
        else if (anchor == 7) // Botton Middle
        {
            realPosition.x = position.x + (parent_corner_pos.x + parent->size.x/2);
            realPosition.y = position.y + (parent_corner_pos.y + parent->size.y);
        }
        else if (anchor == 8)
        {
            realPosition.x = position.x + (parent_corner_pos.x);
            realPosition.y = position.y + (parent_corner_pos.y + parent->size.y);
        }
    }
}

void GameObject::loadTexture(const std::string& image)
{
    texture = ResourceManager::getInstance(SceneManager::renderer).getTexture(image);
    textureName = image;

    SDL_QueryTexture(texture, nullptr, nullptr, &spriteRect.w, &spriteRect.h);

    size.x = spriteRect.w;
    size.y = spriteRect.h;
}

void GameObject::lookAt(const Vector2f& otherpos) 
{
    direction = {otherpos.x - realPosition.x, otherpos.y - realPosition.y};
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        direction.x = direction.x / length;
        direction.y = direction.y / length;
    } else {
        direction = {0.0f, 0.0f};
    }
}

void GameObject::setCentered()
{
    centered = true;
}

bool GameObject::checkBoxCollision(GameObject* otherObject) {
    float left1 = getPosition().x;
    float right1 = getPosition().x + size.x;
    float top1 = getPosition().y;
    float bottom1 = getPosition().y + size.y;

    float left2 = otherObject->getPosition().x;
    float right2 = otherObject->getPosition().x + size.x;
    float top2 = otherObject->getPosition().y;
    float bottom2 = otherObject->getPosition().y + size.y;

    return left1 < right2 && right1 > left2 && top1 < bottom2 && bottom1 > top2;
}

GameObject* GameObject::checkBoxGroupCollision(const std::string& groupName) {
    for (GameObject* object : scene->getGroup(groupName))
    {
        if (checkBoxCollision(object))
        {
            return object;
        }
    }

    return nullptr;
}

bool GameObject::checkCircleCollision(GameObject* otherObject)
{
    float distance = std::sqrt(std::pow(otherObject->realPosition.x - realPosition.x, 2) + 
        std::pow(otherObject->realPosition.y - realPosition.y, 2));
    return distance <= radius + otherObject->radius;
}

GameObject* GameObject::checkCircleGroupCollision(const std::string& groupName)
{
    for (GameObject* object : scene->getGroup(groupName))
    {
        if (checkCircleCollision(object))
        {
            return object;
        }
    }

    return nullptr;
}

void GameObject::addAnimation(std::string title, std::vector<std::string> frames, float fps)
{
    animations[title] = new Animation(this, frames, fps);
}

void GameObject::setAnchor(int anchor_) {
    if (anchor_ >= 0 && anchor_ < 9) {
        anchor = anchor_;
    } else {
        std::cerr << "Invalid anchor, must be between 0-8" << std::endl;
    }
}

Vector2f GameObject::getPosition() {
    return realPosition;
}

