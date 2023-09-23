#pragma once 

#include <SDL.h>
#include "Entity.h"
#include "Component.h"
#include "Math.h"
#include "GameManager.h"

typedef std::vector<Component*> Group;

class CircleCollider : public Component {
    public:
    CircleCollider() : Component("CircleCollider") {};

    void start() override {
        radius = entity->box->getSize().y / 2;
    }

    void update(float deltaTime) override {
        radius = entity->box->getSize().y / 2;
    }

    std::vector<CircleCollider*> getCollisions(std::string tag="") {
        radius = entity->box->getSize().y / 2;
        std::vector<CircleCollider*> hits; 

        for (auto* col : entity->scene->getGroup<CircleCollider>(tag)) {
            if (col->entity == entity) continue;
            if (checkCircleCollision(col)) {
                hits.push_back(col);
            }
        }

        return hits;
    }

    bool checkCollision(CircleCollider* col) {
        return checkCircleCollision(col);
    }

    bool checkCircleCollision(CircleCollider* col) {
        float distance = std::sqrt(std::pow(col->entity->box->getBox().getCenter().x - entity->box->getBox().getCenter().x, 2) + 
                std::pow(col->entity->box->getBox().getCenter().y - entity->box->getBox().getCenter().y, 2));

        //bool c = distance <= radius + col->radius;
        //std::cout << c << std::endl;
        return distance <= radius + col->radius;
    }

    
    float radius = 0;
    bool solid = false;
};

