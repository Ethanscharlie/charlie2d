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
        bool touching =  box->getBox().getLeft()   < InputManager::getMouseUIPosition().x && 
                         box->getBox().getRight()  > InputManager::getMouseUIPosition().x && 
                         box->getBox().getTop()    < InputManager::getMouseUIPosition().y && 
                         box->getBox().getBottom() > InputManager::getMouseUIPosition().y;

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
