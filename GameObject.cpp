#include "GameObject.h"
#include "Scene.h"
#include "ResourceManager.h"

#include <iostream>
#include <vector>
#include "Math.h"

GameObject::GameObject(SDL_Renderer* rendererIn, Scene* sceneIn) : renderer(rendererIn), scene(sceneIn) 
{
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);
}

GameObject::~GameObject() {}

void GameObject::update(float deltaTime) {}

void GameObject::draw() 
{
    spriteRect.x = position.x - centerOffset.x;
    spriteRect.y = position.y - centerOffset.y;
    SDL_RenderCopyEx(renderer, texture, nullptr, &spriteRect, (180 / M_PI) * std::atan2(direction.y, direction.x), nullptr, SDL_FLIP_NONE);
}

void GameObject::loadTexture(const std::string& image)
{
    texture = ResourceManager::getInstance(renderer).getTexture(image);
    SDL_QueryTexture(texture, nullptr, nullptr, &spriteRect.w, &spriteRect.h);
}

void GameObject::lookAt(const Vector2f& otherpos) 
{
    direction = {otherpos.x - position.x, otherpos.y - position.y};
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
    centerOffset.x = spriteRect.w / 2;
    centerOffset.y = spriteRect.h / 2;
}

bool GameObject::checkCircleCollision(GameObject* otherObject)
{
    float distance = std::sqrt(std::pow(otherObject->position.x - position.x, 2) + 
        std::pow(otherObject->position.y - position.y, 2));
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

Vector2f GameObject::getPosition()
{
    return position;
}
