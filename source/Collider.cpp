#include "Collider.h"
#include "EntityBox.h"
#include "Sprite.h"
#include "Entity.h"

void Collider::start() {
    colliderBox = entity->box;
}

void Collider::update(float deltaTime) {
    colliderBox = entity->box;
}

std::vector<Collider*> Collider::getCollisions(std::string tag) {
    colliderBox = entity->box;
    std::vector<Collider*> hits; 

    for (auto* col : GameManager::getComponents<Collider>(tag)) {
        if (col->entity == entity) continue;
        if (checkBoxCollision(col->colliderBox)) {
            hits.push_back(col);
        }
    }

    return hits;
}

bool Collider::checkCollision(Collider col) {
    return checkBoxCollision(col.colliderBox);
}

bool Collider::checkBoxCollision(Box otherBox) {
    if (entity->checkComponent<Sprite>() && !entity->get<Sprite>()->onScreen) {
        return false;
    }

    return colliderBox.getLeft()   < otherBox.getRight() && 
        colliderBox.getRight()  > otherBox.getLeft() && 
        colliderBox.getTop()    < otherBox.getBottom() && 
        colliderBox.getBottom() > otherBox.getTop();
}
