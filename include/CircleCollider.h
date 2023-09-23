#pragma once 

#include <SDL.h>
#include "Entity.h"
#include "Component.h"
#include "Math.h"
#include "SceneManager.h"

typedef std::vector<Component*> Group;

class CircleCollider : public Component {
    public:
    CircleCollider() : Component("CircleCollider") {};

    void start() override {
        radius = object->box->getSize().y / 2;
    }

    void update(float deltaTime) override {
        radius = object->box->getSize().y / 2;
    }

    std::vector<CircleCollider*> getCollisions(std::string tag="") {
        radius = object->box->getSize().y / 2;
        std::vector<CircleCollider*> hits; 

        for (auto* col : object->scene->getGroup<CircleCollider>(tag)) {
            if (col->object == object) continue;
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
        float distance = std::sqrt(std::pow(col->object->box->getBox().getCenter().x - object->box->getBox().getCenter().x, 2) + 
                std::pow(col->object->box->getBox().getCenter().y - object->box->getBox().getCenter().y, 2));

        //bool c = distance <= radius + col->radius;
        //std::cout << c << std::endl;
        return distance <= radius + col->radius;
    }

    
    float radius = 0;
    bool solid = false;
};

