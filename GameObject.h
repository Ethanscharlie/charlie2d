#pragma once
#include <cmath>
#include <SDL.h>
#include <vector>
#include <unordered_map>
#include "Math.h"
#include "Scene.h"
#include "Animation.h"

class Scene;
class Animation;

class GameObject
{
public:
    GameObject();
    virtual ~GameObject();

    void init(Scene* sceneIn, const std::string& groupIn, const int& idIn, GameObject* parentIn);
    virtual void start() = 0;
    virtual void startBase();
    virtual void updateBase(float deltaTime);
    virtual void update(float deltaTime) = 0;
    virtual void draw();
    void move(const Vector2f& change);
    void moveTo(const Vector2f& change);
    void moveToX(float x);
    void moveToY(float y);
    void updateRealPosition();
    virtual void loadTexture(const std::string& image);
    void lookAt(const Vector2f& position);
    void setCentered();
    bool checkBoxCollision(GameObject* otherObject);
    bool checkCircleCollision(GameObject* otherObject);
    GameObject* checkCircleGroupCollision(const std::string& groupName);
    GameObject* checkBoxGroupCollision(const std::string& groupName);
    void addAnimation(std::string title, std::vector<std::string> frames, float fps);

    void setAnchor(int anchor_);
    Vector2f getPosition();
    

    bool toDestroy = false;

    Scene* scene = nullptr;
    std::string group = "";
    int id;
    //bool keepAspect = true;
    GameObject* parent = nullptr;
    Vector2f size = {0, 0};
    Vector2f centerOffset = {0, 0};

protected:
    SDL_Rect spriteRect;
    std::unordered_map<std::string, Animation*> animations;
    SDL_Texture* texture = nullptr;
    std::string textureName = "";
    int anchor = 0;
    Vector2f direction = {0.0f, 0.0f};
    int windowWidth, windowHeight;
    float radius = 0.0f;

private:
    Vector2f position = {0, 0};
    Vector2f realPosition = {0, 0};
    bool centered = false;
    bool debug = true;
};
