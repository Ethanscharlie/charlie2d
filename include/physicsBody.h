#pragma once 

#include "GameObject.h"
#include "Component.h"
#include "Collider.h"
#include "Math.h"

class physicsBody : public Component {
    public:
    physicsBody() : Component("physicsBody") {};

    void start() override {
        //if (object->components->find(typeid(Collider)) != object->components->end()) {
        //    object->addComponent<Collider>();
        //}
    }

    void update(float deltaTime) override {
        //if (velocity.x != 0 || velocity.y != 0) {
        //    Vector2f movement = velocity;
        //    object->box->absoluteBox.position.x += movement.x; 
        //    std::vector<Collider*> cols = object->getComponent<Collider>().getCollisions();

        //    for (Collider* col : cols) {
        //        GameObject* other = col->object;
        //        if (!other->getComponent<Collider>().solid) continue;

        //        if (movement.x > 0) {
        //            object->box->absoluteBox.position.x = (other->box->absoluteBox.getLeft() - object->box->absoluteBox.size.x) ;        
        //            velocity.x = 0;
        //        } else if (movement.x < 0) {
        //            object->box->absoluteBox.position.x = other->box->absoluteBox.getRight();
        //            velocity.x = 0;
        //        }
        //    }

        //    object->box->absoluteBox.position.y += movement.y;
        //    cols = object->getComponent<Collider>().getCollisions();

        //    for (Collider* col : cols) {
        //        GameObject* other = col->object;
        //        if (!other->getComponent<Collider>().solid) continue;

        //        if (movement.y > 0) {
        //            object->box->absoluteBox.position.y = (other->box->absoluteBox.getTop() - object->box->absoluteBox.size.y) ;        
        //            velocity.y = 0;
        //        } else if (movement.y < 0) {
        //            object->box->absoluteBox.position.y = other->box->absoluteBox.getBottom() ;
        //            velocity.y = 0;
        //        }
        //    }
        //    //velocity.print();
        //}

        if (velocity.x != 0 || velocity.y != 0) {
            Vector2f movement = velocity;
            object->box->changePosition({movement.x, 0}); 
            std::vector<Collider*> cols = object->getComponent<Collider>().getCollisions();

            for (Collider* col : cols) {
                GameObject* other = col->object;
                if (!other->getComponent<Collider>().solid) continue;

                if (movement.x > 0) {
                    object->box->setXPosition(other->box->getBox().getLeft() - object->box->getSize().x) ;        
                    velocity.x = 0;
                } else if (movement.x < 0) {
                    object->box->setXPosition(other->box->getBox().getRight());
                    velocity.x = 0;
                }
            }

            object->box->changePosition({0, movement.y});
            cols = object->getComponent<Collider>().getCollisions();

            for (Collider* col : cols) {
                GameObject* other = col->object;
                if (!other->getComponent<Collider>().solid) continue;

                if (movement.y > 0) {
                    object->box->setYPosition(other->box->getBox().getTop() - object->box->getSize().y) ;        
                    velocity.y = 0;
                } else if (movement.y < 0) {
                    object->box->setYPosition(other->box->getBox().getBottom());
                    velocity.y = 0;
                }
            }
            //velocity.print();
        }
    }

    Vector2f velocity;
};

