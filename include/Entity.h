#pragma once
#include <cmath>
#include <SDL.h>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <map>
#include "Math.h"
#include "Scene.h"
#include "Animation.h"
#include "Component.h"
#include "EntityBox.h"

class Scene;
class Animation;
struct entityBox;

class Entity
{
public:
    Entity();
    virtual ~Entity();

    template <typename C>
    void addComponent() {
        C* component = new C();

        component->entity = this;
        component->box = box;
        component->scene = scene;
        component->start();
        components[typeid(C)] = component;
        scene->template add<C>(components[typeid(C)]);
    }

    template <typename C>
    C& getComponent() {
        auto it = components.find(typeid(C));                                                                                                  
        if (it != components.end())                                                                                                            
            return *static_cast<C*>(it->second);                                                                                           
        else                                                                                                                                   
            throw std::runtime_error("Component not found!");  
    }

    template <typename C>
    bool checkComponent() {
        auto it = components.find(typeid(C));
        return it != components.end();
    }

    std::map<std::type_index, Component*> getComponents() {
        return components;
    };
    

    void addChild(Entity* entity);
    void setParent(Entity* entity);
    Entity* getParent();
    std::vector<Entity*> getChildren();
    void removeParent();
    

    bool toDestroy = false;
    Scene* scene = nullptr;
    entityBox* box;
    std::string tag = "";
    bool debug = false;
    int iid;
    bool skipUpdate = false;

    private:
    std::map<std::type_index, Component*> components; 
    std::vector<Entity*> children;
    Entity* parent = nullptr;

};
