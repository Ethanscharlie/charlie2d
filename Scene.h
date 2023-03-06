#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include "GameObject.h"

class GameObject;

typedef std::vector<GameObject*> Group;

class Scene {
public:
    ~Scene();
    void addObject(GameObject* object, const std::string& group);
    void addGroup(const std::string& group);
    void addGroupToLayer(const std::string& group, const int& layer);
    Group& getGroup(const std::string& group);
    void update(float deltaTime);

private:
    std::unordered_map<std::string, Group*> groups;
    std::vector<std::string> layers;
    std::vector<GameObject*> objects;
};
