#pragma once 

#include <SDL.h>
#include "Entity.h"
#include "Component.h"
#include "Math.h"
#include "GameManager.h"
#include "InputManager.h"
#include "EntityBox.h"

/**
 *\brief Meant to be used alongside UISliceRenderer
 */
class Button : public Component {
    public:
    Button() : Component("Button") {};

    void start() override {
    }

    void update(float deltaTime) override {
        bool touching =  entity->require<entityBox>()->getBox().getLeft()   < InputManager::getMouseUIPosition().x && 
                         entity->require<entityBox>()->getBox().getRight()  > InputManager::getMouseUIPosition().x && 
                         entity->require<entityBox>()->getBox().getTop()    < InputManager::getMouseUIPosition().y && 
                         entity->require<entityBox>()->getBox().getBottom() > InputManager::getMouseUIPosition().y;

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
