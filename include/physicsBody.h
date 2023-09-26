#pragma once 

#include "Entity.h"
#include "Component.h"
#include "Collider.h"
#include "Math.h"

class physicsBody : public Component {
    public:
    physicsBody() : Component("physicsBody") {};

    void start() override {
        //if (entity->components->find(typeid(Collider)) != entity->components->end()) {
        //    entity->addComponent<Collider>();
        //}
    }

    void update(float deltaTime) override {
        velocity.y += gravity * deltaTime;

        if (velocity.x != 0 || velocity.y != 0) {
            Vector2f movement = velocity * deltaTime;
            entity->box->changePosition({movement.x, 0}); 
            std::vector<Collider*> cols = entity->getComponent<Collider>().getCollisions();

            for (Collider* col : cols) {
                Entity* other = col->entity;
                if (!other->getComponent<Collider>().solid) continue;

                if (movement.x > 0) {
                    entity->box->setXPosition(other->box->getBox().getLeft() - entity->box->getSize().x) ;        
                    velocity.x = 0;
                } else if (movement.x < 0) {
                    entity->box->setXPosition(other->box->getBox().getRight());
                    velocity.x = 0;
                }
            }

            entity->box->changePosition({0, movement.y});
            cols = entity->getComponent<Collider>().getCollisions();

            for (Collider* col : cols) {
                Entity* other = col->entity;
                if (!other->getComponent<Collider>().solid) continue;

                if (movement.y > 0) {
                    entity->box->setYPosition(other->box->getBox().getTop() - entity->box->getSize().y) ;        
                    velocity.y = 0;
                } else if (movement.y < 0) {
                    entity->box->setYPosition(other->box->getBox().getBottom());
                    velocity.y = 0;
                }
            }
            //velocity.print();
        }
    }

    Vector2f velocity;
    float gravity = 0;
};

