#include "Scene.h"

Scene::~Scene() {
    // destroy all objects
}

void Scene::addObject(GameObject* object, const std::string& group)
{
    if (groups.count(group) == 0) {
        addGroup(group);
    }
    groups[group]->push_back(object);
}

void Scene::addGroup(const std::string& group)
{
    groups[group] = new Group();
    layers.push_back(group);
}

void Scene::addGroupToLayer(const std::string& group, const int& layer )
{
    groups[group] = new Group();
    layers.insert(layers.begin() + layer, group);
}

Group& Scene::getGroup(const std::string& group)
{
    return *(groups[group]);
}

void Scene::update(float deltaTime)
{
    for (std::string& layer : layers) {
        Group* group = groups[layer];
        for (auto it = group->begin(); it != group->end(); ) {
            GameObject* object = *it;
            object->update(deltaTime);
            object->draw();
            if (object->toDestroy) {
                it = group->erase(it);
                delete object;
            } else {
                ++it;
            }
        }
    }
}

// std::vector<GameObject*> Scene::getAllObjects() {
//     std::vector<GameObject*> allObjects;
//     for (Group& group : groups) {
//         for (GameObject* obj : group.objects) {
//             allObjects.push_back(obj);
//         }
//     }
//     return allObjects;
// }

// void Scene::addObject(const std::string& name, GameObject* object) 
// {
//     // Check if name exists in map
//     Group& group = findGroup(name);
//     // Add object to vector
//     group.objects.push_back(object);
//     objects.push_back(object);
//     object->groups.push_back(name);
// }

// void Scene::removeObject(GameObject* object)
// {
//     // Remove the object from the groups it belongs to
//     for (const std::string& groupName : object->groups)
//     {
//         Group& group = findGroup(groupName);
//         group.objects.erase(std::remove(group.objects.begin(), group.objects.end(), object), group.objects.end());
//     } 

//     objects.erase(std::remove(objects.begin(), objects.end(), object), objects.end());  

// }

// Group& Scene::findGroup(const std::string& name) 
// {
//     for (Group& group : groups)
//     {
//         if (group.name == name)
//         {
//             return group;
//         }
//     }

//     // If group with given name not found, create it and return it
//     groups.push_back({name, std::vector<GameObject*>()});
//     return groups.back();
// }

// std::vector<GameObject*>& Scene::getObjects(const std::string& name) {
//     // Return vector of objects for given key
//     return findGroup(name).objects;
// }
