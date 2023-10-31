#pragma once
#include "GameManager.h"
#include "Entity.h"

class JumpMan : public Component {
    public:
    void start() override {
        //entity->getComponent<physicsBody>().maxVelocity.x = 200;
    }

    void update(float deltaTime) override {
        if (abs(entity->getComponent<physicsBody>().velocity.x) < 200) {
            entity->getComponent<physicsBody>().velocity.x += InputManager::checkHorizontal() * speed;
        }

        if(abs(entity->getComponent<physicsBody>().velocity.x) < 1) {
            entity->getComponent<physicsBody>().velocity.x = 0;
        } else {
            //entity->getComponent<physicsBody>().velocity.x -= 
            //    entity->getComponent<physicsBody>().velocity.x/abs(entity->getComponent<physicsBody>().velocity.x) * tracktion * deltaTime;
            if (entity->getComponent<physicsBody>().velocity.x > 0) {
                entity->getComponent<physicsBody>().velocity.x -= tracktion * deltaTime;
            } else {
                entity->getComponent<physicsBody>().velocity.x += tracktion * deltaTime;
            }
        }

        if (InputManager::checkInput("jumpTrigger")) {
            if (!allowJump) return;

            bool checkground = false;
            if (needGround) {
                groundCheckBox = entity->box->getBox();
                groundCheckBox.position.y += entity->box->getSize().y;
                for (Collider* col : entity->scene->getGroup<Collider>()) {
                    if (col->solid) {
                        if (col->checkBoxCollision(groundCheckBox)) {
                            checkground = true;
                            break;
                        }
                    }
                }
            } else checkground = true;

            if (checkground || jumpsCounter > 0) {
                if (checkground) jumpsCounter = jumps;
                jumpsCounter --;
                // Start Jump
                if (!jumping) entity->getComponent<physicsBody>().velocity.y = 0;
                jumping = true;
                gravity = 1500;
            }
        } 

        entity->getComponent<physicsBody>().velocity.y += gravity * deltaTime;

        if (InputManager::checkInput("jump")) {
            // Keep Jump 
            if (jumping) {
                if (jumpAmount <= jumpPeak) {
                    entity->getComponent<physicsBody>().velocity.y = -jumpChange;
                    jumpAmount += jumpChange * deltaTime;
                }
            }
        } else {
            jumpAmount = 0;
            jumping = false;
        }

    }

    float gravity = 1500;
    //float gravity = 0;
    float speed = 40;
    float tracktion = 1000;
    float jumpPeak = 80;
    float jumpChange = 300;
    int jumps = 1;

    bool allowJump = true;
    bool needGround = true;

    private:
    float jumpsCounter;
    float jumpAmount = 0;
    bool jumping = false;
    Box groundCheckBox = {0, 0, 0, 0};
};

