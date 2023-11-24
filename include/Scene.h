#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <typeindex>
#include <map>
#include <unordered_map>
#include "GameManager.h"

#include "Component.h"
//#include "Entity.h"

typedef std::vector<Component*> Group;

class Entity;

class Scene {
public:
    virtual ~Scene();
    Scene();
    //void add(Entity* entity, const std::string& group);

    template <typename C>
    void add(Component* component) {

        //if (components.count(typeid(C)) == 0) { //&& std::find(componentTypes.begin(), componentTypes.end(), typeid(C)) == componentTypes.end()) {
        //    componentTypes.push_back(typeid(C));
        //    std::cout << "added type " << component->title << std::endl;
        //}

        //component->index = 0;//components[typeid(C)].size();
        //components[typeid(C)].push_back(component);
    }

    template <typename C>                                                                                                                    
    std::vector<C*> getGroup(std::string tag = "") {                                                                                                             
        std::vector<C*> hits;                                                                                                                
        auto it = components.find(typeid(C));                                                                                                

        if (it != components.end()) {                                                                                                        
            for (auto component : it->second) {                                                                                              
                // Static cast each Component* to C*                                                                                         
                if (component->entityTag == tag || tag == "") {
                    hits.push_back(static_cast<C*>(component));                                                                                  
                }
            }                                                                                                                                
        }                                                                                                                                    
        else                                                                                                                                 
            components[typeid(C)];
            // throw std::runtime_error("Component group not found!");                                                                                

        return hits;                                                                                                                         
    }                                                                                                                                        

    std::vector<Entity*> getTag(std::string tag);

    Entity* createEntity(std::string tag="");
    //void addToParent(Entity* entity, Entity* parent);
    std::vector<Entity*> getAllObjects();
    void update();
    virtual void load() = 0;
    void unload();

    bool updateEntities = true;
    float deltaTime = 0;


//private:
    Uint64 lastTime;
    std::unordered_map<std::string, std::vector<Entity*>> tags;
    std::vector<Entity*> allObjects;
    std::map<std::type_index, Group> components; 
    //std::vector<std::string> layers;
    //std::vector<Entity*> entitys;
};

#endif
