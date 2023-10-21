#ifndef COMPONENT_H
#define COMPONENT_H

#include <iostream>

//#include "Entity.h"
//#include "EntityBox.h"

class Scene;
struct entityBox;
class Entity;

class Component {
    public:
    Component(std::string _title="") : title(_title) {};
    virtual void start() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void onDestroy() {}

    
    //template <typename C>
    //void addComponent() {
    //    return entity->template addComponent<C>();
    //}

    //template <typename C>
    //C& getComponent() {
    //    return entity->template getComponent<C>();
    //}
    //template <typename C>
    //void addComponent() {
    //    C* component = new C();

    //    component->entity = this;
    //    component->box = box;
    //    component->scene = scene;
    //    component->start();
    //    entity->components[typeid(C)] = component;
    //    scene->template add<C>(entity->getComponents()[typeid(C)]);
    //}

    //template <typename C>
    //C& getComponent() {
    //    auto it = entity->getComponents().find(typeid(C));                                                                                                  
    //    if (it != entity->getComponents().end())                                                                                                            
    //        return *static_cast<C*>(it->second);                                                                                           
    //    else                                                                                                                                   
    //        throw std::runtime_error("Component not found!");  
    //}

    template <typename C>
    bool checkComponent() {
        return checkComponent<C>();
    }

    entityBox* box;
    Entity* entity = nullptr;
    Scene* scene;
    std::string title = "";
    bool isInit = false;
    int layer = 0;
    int index;
    std::string entityTag = "";

    bool useLayer = false;
    bool standardUpdate = true;
};

#endif 

