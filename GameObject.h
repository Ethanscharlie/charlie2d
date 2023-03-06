#pragma once
#include <cmath>
#include <SDL.h>
#include <vector>
#include "Math.h"
#include "Scene.h"

class Scene;

class GameObject
{
public:
    GameObject(SDL_Renderer* rendererIn, Scene* sceneIn);
    virtual ~GameObject();

    virtual void update(float deltaTime) = 0;
    virtual void draw();
    void loadTexture(const std::string& image);
    void lookAt(const Vector2f& position);
    void setCentered();
    bool checkCircleCollision(GameObject* otherObject);
    GameObject* checkCircleGroupCollision(const std::string& groupName);

    Vector2f getPosition();

    bool toDestroy = false;

protected:
    SDL_Renderer* renderer;
    Scene* scene;
    std::string group;
    Vector2f position = {0, 0};
    Vector2f centerOffset = {0, 0};
    SDL_Texture* texture;
    SDL_Rect spriteRect;
    Vector2f direction = {0.0f, 0.0f};
    int windowWidth, windowHeight;
    float radius = 0.0f;
};