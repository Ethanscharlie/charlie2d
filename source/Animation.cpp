#include "Animation.h"
#include "Image.h"
#include "Sprite.h"

Animation::Animation(Sprite* spriteIn, std::vector<Image> framesIn, float fpsIn) : sprite(spriteIn), frames(framesIn), fps(fpsIn) 
{
    
    
    
}

void Animation::update()
{
    if (playing && timer + fps*1000 < SDL_GetTicks())
    {
        sprite->image = frames[frameCounter];
        
        if (direction == 1)
        {
            frameCounter ++;
        }
        else if(direction == -1)
        {
            frameCounter --;
        }

        if ((direction == 1 && frames.size() == frameCounter) ||
                (direction == -1 && frameCounter == -1)) 
        {
            if (looping) 
            {
                resetFrameCounter();
            }
            else {
                playing = false;
                onFinish();
                onFinish = [](){}; 
            }
        }

        timer = SDL_GetTicks();
    }
}

void Animation::resetFrameCounter()
{
    if (direction == 1)
    {
        frameCounter = 0;
    }
    else if (direction == -1)
    {
        frameCounter = frames.size()-1;
    }
}

void Animation::play(int directionIn, std::function<void()> onFinishIn)
{
    playing = true;
    looping = false;
    direction = directionIn;
    onFinish = onFinishIn;
    resetFrameCounter();
}

void Animation::loop(int directionIn)
{
    playing = true;
    looping = true;
    direction = directionIn;
    resetFrameCounter();
}
