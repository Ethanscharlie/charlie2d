#pragma once 

#include <SDL.h>
#include "Collider.h"
#include "Math.h"
#include "GameManager.h"
#include "Animation.h"

class Animation;
class Entity;

class Sprite : public Component {
    public:
    Sprite() : Component("sprite") {};

    void start() override {
        useLayer = true;
    }

    void update(float deltaTime) override;

    void loadTexture(const std::string& image, bool setSize=true, bool keepCentered=true);
    void addAnimation(std::string title, std::vector<std::string> frames, float fps);

    SDL_Rect sourceRect = {0, 0, 0, 0};
    SDL_Texture* texture = nullptr;
    std::string textureName = "";
    bool showBorders = false;
    Angle angle;
    std::unordered_map<std::string, Animation*> animations;
    bool onScreen = false;
    bool renderAsUI = false;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
};

