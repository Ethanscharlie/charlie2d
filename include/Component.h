#pragma once
#include "Entity.h"
#include "EntityBox.h"

class Entity;
class Scene;
struct entityBox;

class Component {
    public:
    Component(std::string _title) : title(_title) {};
    virtual void start() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void onDestroy() {}

    
    template <typename C>
    void addComponent() {
        return entity->template addComponent<C>();
    }

    template <typename C>
    C& getComponent() {
        return entity->template getComponent<C>();
    }

    template <typename C>
    bool checkComponent() {
        return checkComponent<C>();
    }

    entityBox* box;
    Entity* entity;
    Scene* scene;
    std::string title = "";
    bool isInit = false;
    int layer = 0;
    int index;

    bool useLayer = false;
    bool standardUpdate = true;
};

