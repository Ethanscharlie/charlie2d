#include "Entity.h"
#include "Scene.h"
#include "ResourceManager.h"

#include <iostream>
#include <vector>
#include "Math.h"

Entity::Entity() {
    box = new objectBox(this);
}

Entity::~Entity() {
}

void Entity::addChild(Entity* object) {
    children.push_back(object);
}

void Entity::setParent(Entity* object) {
    parent = object;
    object->addChild(this);
}

std::vector<Entity*> Entity::getChildren() {
    return children;
}

Entity* Entity::getParent() {
    return parent;
}

void Entity::removeParent() {

}

//void Entity::setAnchor(int anchor_) {
//    if (anchor_ >= 0 && anchor_ < 9) {
//        anchor = anchor_;
//    } else {
//        std::cerr << "Invalid anchor, must be between 0-8" << std::endl;
//    }
//}

