#pragma once 

#include <SDL.h>
#include "Entity.h"
#include "Component.h"
#include "Math.h"
#include "GameManager.h"
#include "InputManager.h"

class Button : public Component {
    public:
    Button() : Component("Button") {};

    void start() override {
    }

    void update(float deltaTime) override {
        bool touching =  box->getBox().getLeft()   < InputManager::getMouseScreenPosition().x && 
                         box->getBox().getRight()  > InputManager::getMouseScreenPosition().x && 
                         box->getBox().getTop()    < InputManager::getMouseScreenPosition().y && 
                         box->getBox().getBottom() > InputManager::getMouseScreenPosition().y;

        if (touching) {
            onHover();
            if(InputManager::checkInput("fire")) {
                onClick();
            }
        } else {
            offHover();
        }
    }

    std::function<void()> onClick = [](){};
    std::function<void()> onHover = [](){};
    std::function<void()> offHover = [](){};
};
