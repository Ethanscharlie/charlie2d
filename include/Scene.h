#pragma once

#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <unordered_map>
#include "SceneManager.h"
#include "GameObject.h"
#include "Component.h"
//#include "Sprite.h"

class GameObject;

typedef std::vector<Component*> Group;

class Scene {
public:
    ~Scene();
    Scene();
    //void add(GameObject* object, const std::string& group);

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
                if (component->object->tag == tag || tag == "") {
                    hits.push_back(static_cast<C*>(component));                                                                                  
                }
            }                                                                                                                                
        }                                                                                                                                    
        else                                                                                                                                 
            throw std::runtime_error("Component not found!");                                                                                

        return hits;                                                                                                                         
    }                                                                                                                                        

    std::vector<GameObject*> getTag(std::string tag);

    GameObject* createObject(std::string tag="");
    //void addToParent(GameObject* object, GameObject* parent);
    std::vector<GameObject*> getAllObjects();
    void update();
    virtual void load();
    void unload();

    int windowWidth, windowHeight;


private:
    Uint64 lastTime;
    std::unordered_map<std::string, std::vector<GameObject*>> tags;
    std::vector<GameObject*> allObjects;
    std::map<std::type_index, Group> components; 
    std::vector<std::type_index> componentTypes;
    //std::vector<std::string> layers;
    //std::vector<GameObject*> objects;
};
