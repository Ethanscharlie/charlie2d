#pragma once

#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <unordered_map>
#include "SceneManager.h"
#include "Entity.h"
#include "Component.h"
//#include "Sprite.h"

class Entity;

typedef std::vector<Component*> Group;

class Scene {
public:
    ~Scene();
    Scene();
    //void add(Entity* entity, const std::string& group);

    template <typename C>
    void add(Component* component) {

        if (components.count(typeid(C)) == 0) { //&& std::find(componentTypes.begin(), componentTypes.end(), typeid(C)) == componentTypes.end()) {
            componentTypes.push_back(typeid(C));
            std::cout << "added type " << component->title << std::endl;
        }

        component->index = 0;//components[typeid(C)].size();
        components[typeid(C)].push_back(component);
    }

    template <typename C>                                                                                                                    
    std::vector<C*> getGroup(std::string tag = "") {                                                                                                             
        std::vector<C*> hits;                                                                                                                
        auto it = components.find(typeid(C));                                                                                                

        if (it != components.end()) {                                                                                                        
            for (auto component : it->second) {                                                                                              
                // Static cast each Component* to C*                                                                                         
                if (component->entity->tag == tag || tag == "") {
                    hits.push_back(static_cast<C*>(component));                                                                                  
                }
            }                                                                                                                                
        }                                                                                                                                    
        else                                                                                                                                 
            throw std::runtime_error("Component not found!");                                                                                

        return hits;                                                                                                                         
    }                                                                                                                                        

    std::vector<Entity*> getTag(std::string tag);

    Entity* createObject(std::string tag="");
    //void addToParent(Entity* entity, Entity* parent);
    std::vector<Entity*> getAllObjects();
    void update();
    virtual void load();
    void unload();

    int windowWidth, windowHeight;


private:
    Uint64 lastTime;
    std::unordered_map<std::string, std::vector<Entity*>> tags;
    std::vector<Entity*> allObjects;
    std::map<std::type_index, Group> components; 
    std::vector<std::type_index> componentTypes;
    //std::vector<std::string> layers;
    //std::vector<Entity*> entitys;
};
