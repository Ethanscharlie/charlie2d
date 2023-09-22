#include "Collider.h"

void Collider::start() override {
    colliderBox = object->box->getBox();
}

void Collider::update(float deltaTime) override {
    colliderBox = object->box->getBox();
}

std::vector<Collider*> Collider::getCollisions(std::string tag) {
    colliderBox = object->box->getBox();
    std::vector<Collider*> hits; 

    for (auto* col : object->scene->getGroup<Collider>(tag)) {
        if (col->object == object) continue;
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
