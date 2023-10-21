#include "EntityBox.h"
#include "Scene.h"
#include "Entity.h"

entityBox::entityBox(Entity* _entity)
:entity(_entity)
{}

void entityBox::print() {
    globalBox.print();
    localBox.print();
}

void entityBox::updateGlobalBox() {
    globalBox.size = localBox.size;

    if (entity->getParent() == nullptr) {
        globalBox.position = localBox.position;
    } else {
        if (anchor == 0) // Top Left
        {
            globalBox.position = localBox.position + entity->getParent()->box->globalBox.getTopLeftCorner();

        }
        else if (anchor == 1) // Top Middle
        {
            globalBox.position = localBox.position + entity->getParent()->box->globalBox.getTopMiddle();
        }
        else if (anchor == 2)
        {
            globalBox.position = localBox.position + entity->getParent()->box->globalBox.getTopRightCorner();

        }
        else if (anchor == 3)
        {
        }
        else if (anchor == 4) // Middle
        {
            globalBox.position = localBox.position + entity->getParent()->box->globalBox.getCenter();
        }
        else if (anchor == 5)
        {

        }
        else if (anchor == 6)
        {
            globalBox.position = localBox.position + entity->getParent()->box->globalBox.getBottomLeftCorner();
        }
        else if (anchor == 7) // Botton Middle
        {
            globalBox.position = localBox.position + entity->getParent()->box->globalBox.getBottomMiddle(); 
        }
        else if (anchor == 8)
        {
            globalBox.position = localBox.position + entity->getParent()->box->globalBox.getBottomRightCorner();
        }

        for (Entity* child : entity->getChildren()) { 
            child->box->updateGlobalBox();
        }
    }
}

void entityBox::updateLocalBox() {
    localBox.size = globalBox.size;

    if (entity->getParent() == nullptr) {
        localBox.position = globalBox.position;
    } else {
        if (anchor == 0) // Top Left
        {
            localBox.position = globalBox.position - entity->getParent()->box->globalBox.getTopLeftCorner();

        }
        else if (anchor == 1) // Top Middle
        {
            localBox.position = globalBox.position - entity->getParent()->box->globalBox.getTopMiddle();
        }
        else if (anchor == 2)
        {
            localBox.position = globalBox.position - entity->getParent()->box->globalBox.getTopRightCorner();

        }
        else if (anchor == 3)
        {
        }
        else if (anchor == 4) // Middle
        {
            localBox.position = globalBox.position - entity->getParent()->box->globalBox.getCenter();
        }
        else if (anchor == 5)
        {

        }
        else if (anchor == 6)
        {
            localBox.position = globalBox.position - entity->getParent()->box->globalBox.getBottomLeftCorner();
        }
        else if (anchor == 7) // Botton Middle
        {
            localBox.position = globalBox.position - entity->getParent()->box->globalBox.getBottomMiddle(); 
        }
        else if (anchor == 8)
        {
            localBox.position = globalBox.position - entity->getParent()->box->globalBox.getBottomRightCorner();
        }
    }

    for (Entity* child : entity->getChildren()) { 
        child->box->updateGlobalBox();
    }
}
