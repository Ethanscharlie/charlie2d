#pragma once

#include "ResourceManager.h"
#include "Entity.h"
#include "Component.h"
#include "Sprite.h"
#include <functional>

class Entity;
class Sprite;

class Animation
{
    public:
    Animation(Sprite* spriteIn, std::vector<std::string> framesIn, float fpsIn);
    void update();
    void resetFrameCounter();
    void play(int direction=1, std::function<void()> onFinishIn=[](){});
    void loop(int direction=1);

    Sprite* sprite;
    std::vector<std::string> frames;
    Uint32 timer = SDL_GetTicks();
    float fps = 1;
    int frameCounter = 0;
    int direction = 1;
    bool playing;
    bool looping;
    std::function<void()> onFinish;
};
