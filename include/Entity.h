#ifndef ENTITY_H
#define ENTITY_H
#include <cmath>
#include <SDL.h>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <map>
#include "Math.h"
#include "Scene.h"
//#include "Component.h"
//#include "EntityBox.h"

class Scene;
class Animation;
struct entityBox;
class Component;

class Entity
{
public:
    Entity() {
        //box = new entityBox(this);
    };

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
    

    void addChild(Entity* entity) {
        children.push_back(entity);
    }

    void setParent(Entity* entity) {
        parent = entity;
        entity->addChild(this);
    }

    Entity* getParent() {return parent;}
    std::vector<Entity*> getChildren() {return children;}
    void removeParent() {};
    

    bool toDestroy = false;
    Scene* scene = nullptr;
    entityBox* box;
    std::string tag = "";
    bool debug = false;
    int iid;
    bool skipUpdate = false;
    bool active = true;

    std::map<std::type_index, Component*> components; 
    private:
    std::vector<Entity*> children;
    Entity* parent = nullptr;

};

#endif // ENTITY_H
