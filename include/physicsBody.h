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
        //if (velocity.x != 0 || velocity.y != 0) {
        //    Vector2f movement = velocity;
        //    entity->box->absoluteBox.position.x += movement.x; 
        //    std::vector<Collider*> cols = entity->getComponent<Collider>().getCollisions();

        //    for (Collider* col : cols) {
        //        Entity* other = col->entity;
        //        if (!other->getComponent<Collider>().solid) continue;

        //        if (movement.x > 0) {
        //            entity->box->absoluteBox.position.x = (other->box->absoluteBox.getLeft() - entity->box->absoluteBox.size.x) ;        
        //            velocity.x = 0;
        //        } else if (movement.x < 0) {
        //            entity->box->absoluteBox.position.x = other->box->absoluteBox.getRight();
        //            velocity.x = 0;
        //        }
        //    }

        //    entity->box->absoluteBox.position.y += movement.y;
        //    cols = entity->getComponent<Collider>().getCollisions();

        //    for (Collider* col : cols) {
        //        Entity* other = col->entity;
        //        if (!other->getComponent<Collider>().solid) continue;

        //        if (movement.y > 0) {
        //            entity->box->absoluteBox.position.y = (other->box->absoluteBox.getTop() - entity->box->absoluteBox.size.y) ;        
        //            velocity.y = 0;
        //        } else if (movement.y < 0) {
        //            entity->box->absoluteBox.position.y = other->box->absoluteBox.getBottom() ;
        //            velocity.y = 0;
        //        }
        //    }
        //    //velocity.print();
        //}

        if (velocity.x != 0 || velocity.y != 0) {
            Vector2f movement = velocity;
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
};

