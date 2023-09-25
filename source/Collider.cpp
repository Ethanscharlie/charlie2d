#include "Collider.h"

void Collider::start() {
    colliderBox = entity->box->getBox();
}

void Collider::update(float deltaTime) {
    colliderBox = entity->box->getBox();
}

std::vector<Collider*> Collider::getCollisions(std::string tag) {
    colliderBox = entity->box->getBox();
    std::vector<Collider*> hits; 

    for (auto* col : entity->scene->getGroup<Collider>(tag)) {
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
    return colliderBox.getLeft()   < otherBox.getRight() && 
        colliderBox.getRight()  > otherBox.getLeft() && 
        colliderBox.getTop()    < otherBox.getBottom() && 
        colliderBox.getBottom() > otherBox.getTop();
}
