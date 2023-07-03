#include "Scene.h"

Scene::~Scene() {
    // destroy all objects
}

Scene::Scene()
{   
    SDL_GetRendererOutputSize(SceneManager::renderer, &windowWidth, &windowHeight);
}

void Scene::add(GameObject* object, const std::string& group)
{
    object->init(this, group, groups[group]->size(), nullptr);
    object->startBase();
    object->start();

    if (groups.count(group) == 0) {
        addGroup(group);
    }
    groups[group]->push_back(object);
}

void Scene::addToParent(GameObject* object, GameObject* parent)
{
    if (parent == nullptr) {std::cout << "nullparent" << std::endl; return;}
    std::cout << parent << std::endl;
    object->init(this, parent->group, parent->id + 1, parent);
    object->startBase();
    object->start();

    auto insert_pos = groups[parent->group]->begin() + parent->id + 1;
    groups[parent->group]->emplace(insert_pos, object);
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

void Scene::update()
{
    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    SDL_SetRenderDrawColor(SceneManager::renderer, 19, 14, 77, 255);
    SDL_RenderClear(SceneManager::renderer);

    lastTime = SDL_GetTicks();

    for (std::string& layer : layers) {
        Group* group = groups[layer];
        for (auto it = group->begin(); it != group->end(); ) {
            GameObject* object = *it;
            object->updateBase(deltaTime);
            object->update(deltaTime);
            object->draw();
            
            // FPS
            // double fps;
            // float msec = SDL_GetTicks() - currentTime;
            // if(msec > 0)
            //     fps = 1000.0 / (double) msec;
            // std::cout << "FPS: " << fps << std::endl;

            // Destory Objects
            if (object->toDestroy) {
                it = group->erase(it);
                delete object;
            } else {
                ++it;
            }
        }
    }

    //SDL_RenderPresent(SceneManager::renderer);


}

void Scene::unload()
{
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
    objects.clear();
    layers.clear();
}

void Scene::load() {}


