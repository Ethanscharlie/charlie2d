#pragma once

#include "ResourceManager.h"
#include "GameObject.h"
#include <functional>

class GameObject;

class Animation
{
    public:
    Animation(GameObject* objectIn, std::vector<std::string> framesIn, float fpsIn);
    void update();
    void resetFrameCounter();
    void play(int direction=1, std::function<void()> onFinishIn=[](){});
    void loop(int direction=1);

    GameObject* object;
    std::vector<std::string> frames;
    Uint32 timer = SDL_GetTicks();
    float fps = 1;
    int frameCounter = 0;
    int direction = 1;
    bool playing;
    bool looping;
    std::function<void()> onFinish;
};
