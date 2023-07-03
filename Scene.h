#pragma once

#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <unordered_map>
#include "SceneManager.h"
#include "GameObject.h"

class GameObject;

typedef std::vector<GameObject*> Group;

class Scene {
public:
    ~Scene();
    Scene();
    void add(GameObject* object, const std::string& group);
    void addToParent(GameObject* object, GameObject* parent);
    void addGroup(const std::string& group);
    void addGroupToLayer(const std::string& group, const int& layer);
    Group& getGroup(const std::string& group);
    void update();
    virtual void load();
    void unload();

    int windowWidth, windowHeight;

private:
    Uint32 lastTime;
    std::unordered_map<std::string, Group*> groups;
    std::vector<std::string> layers;
    std::vector<GameObject*> objects;
};
