#include "GameObject.h"
#include "Scene.h"
#include "ResourceManager.h"

#include <iostream>
#include <vector>
#include "Math.h"

GameObject::GameObject() {
    box = new objectBox(this);
}

GameObject::~GameObject() {
}

void GameObject::addChild(GameObject* object) {
    children.push_back(object);
}

void GameObject::setParent(GameObject* object) {
    parent = object;
    object->addChild(this);
}

std::vector<GameObject*> GameObject::getChildren() {
    return children;
}

GameObject* GameObject::getParent() {
    return parent;
}

void GameObject::removeParent() {

}

//void GameObject::setAnchor(int anchor_) {
//    if (anchor_ >= 0 && anchor_ < 9) {
//        anchor = anchor_;
//    } else {
//        std::cerr << "Invalid anchor, must be between 0-8" << std::endl;
//    }
//}

