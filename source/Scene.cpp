#include "Scene.h"

Scene::~Scene() {
    // destroy all objects
}

Scene::Scene()
{   
    SDL_GetRendererOutputSize(SceneManager::renderer, &windowWidth, &windowHeight);
}

GameObject* Scene::createObject(std::string tag) {
    GameObject* object = new GameObject();
    object->scene = this;
    object->tag = tag;
    object->iid = allObjects.size();
    tags[tag].push_back(object);
    allObjects.push_back(object);
    return object;
}


std::vector<GameObject*> Scene::getTag(std::string tag) {
    return tags[tag];
}

void Scene::update()
{
    Uint64 currentTime = SDL_GetPerformanceCounter();
    //float deltaTime = (currentTime - lastTime) / 1000.0f;
    float deltaTime = static_cast<float>((currentTime - lastTime) * 1000 /
            static_cast<double>(SDL_GetPerformanceFrequency())) * 0.001;

    if (deltaTime > 900) deltaTime = 0;

    lastTime = currentTime;
    lastTime = SDL_GetPerformanceCounter();

    //std::cout << "FPS: " << 1.0f / deltaTime << std::endl;

    SDL_SetRenderDrawColor(SceneManager::renderer, 0, 0, 0, 255);
    SDL_RenderClear(SceneManager::renderer);

    std::vector<Component*> layeredComponents;
    for (auto& c : components) {
        std::vector<Component*>* clist = &c.second;
        //if (clist->size() > 0 && clist->data()[0]->useLayer) {
        //    //layeredComponents->insert(layeredComponents->end(), clist->begin(), clist->end());
        //    for (auto it = clist->begin(); it != clist->end();) {
        //        Component* component = *it;
        //        layeredComponents.push_back(component);
        //        ++it;
        //    }
        //    
        //    //continue;
        //}

        for (auto it = clist->begin(); it != clist->end();) {
            Component* component = *it;
            if (component == nullptr) continue;
            GameObject* object = component->object;

            if (object->toDestroy) {
                component->onDestroy();
                it = clist->erase(it);

                std::remove(allObjects.begin(), allObjects.end(), object);
            } else {
                if(component->useLayer) {
                    layeredComponents.push_back(component);
                } else {
                    component->update(deltaTime);
                }

                ++it;
            }
        }
    }

    std::sort(layeredComponents.begin(), layeredComponents.end(), [](Component* a, Component* b){
            if (a->layer == b->layer) return a->object->iid < b->object->iid;
            return a->layer < b->layer;
            });

    for (Component* component : layeredComponents) {
        component->update(deltaTime);
    }

    //for (auto it = layeredComponents->begin(); it != layeredComponents->end();) {
    //    Component* component = *it;
    //    if (component == nullptr) continue;
    //    GameObject* object = component->object;

    //    if (object->toDestroy) {
    //        component->onDestroy();
    //        it = layeredComponents->erase(it);

    //        std::remove(allObjects.begin(), allObjects.end(), object);
    //    } else {
    //        component->update(deltaTime);
    //        ++it;
    //    }
    //}

    //for (auto& c : components) {
    //    std::vector<Component*>* clist = &c.second;
}

std::vector<GameObject*> Scene::getAllObjects() {
    return allObjects;
}

void Scene::unload()
{
    componentTypes.clear();
    components.clear();
    tags.clear();
    // for (auto& pair : groups) {
    //     Group* group = pair.second;
    //     for (auto it = group->begin(); it != group->end(); ) {
    //         GameObject* object = *it;
    //         if (object) {
    //             delete object;
    //             it = group->erase(it);
    //         } else {
    //             ++it;
    //         }
    //     }
    //     delete group;
    // }
    //groups.clear();
    //objects.clear();
    //layers.clear();
}

void Scene::load() {}


