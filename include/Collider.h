#ifndef COLLIDER_H
#define COLLIDER_H

#include <SDL.h>
#include "Entity.h"
#include "Component.h"
#include "Math.h"
#include "GameManager.h"
#include "EntityBox.h"

typedef std::vector<Component*> Group;

class Collider : public Component {
    public:
    Collider() : Component("collider") {};

    void start() override;
    void update(float deltaTime) override;
    std::vector<Collider*> getCollisions(std::string tag="");
    bool checkCollision(Collider col);
    bool checkBoxCollision(Box otherBox);
    
    Box colliderBox; 
    bool solid = false;
};

#endif
