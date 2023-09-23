#pragma once
#include "Entity.h"
#include "ObjectBox.h"

class Entity;
class Scene;
struct objectBox;

class Component {
    public:
    Component(std::string _title) : title(_title) {};
    virtual void start() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void onDestroy() {}

    
    template <typename C>
    void addComponent() {
        return object->template addComponent<C>();
    }

    template <typename C>
    C& getComponent() {
        return object->template getComponent<C>();
    }

    template <typename C>
    bool checkComponent() {
        return checkComponent<C>();
    }

    objectBox* box;
    Entity* object;
    Scene* scene;
    std::string title = "";
    bool isInit = false;
    int layer = 0;
    int index;

    bool useLayer = false;
};

