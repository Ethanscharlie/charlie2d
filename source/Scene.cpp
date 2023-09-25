#include "Scene.h"

Scene::~Scene() {
    // destroy all entitys
}

Scene::Scene()
{   
    SDL_GetRendererOutputSize(GameManager::renderer, &windowWidth, &windowHeight);
}

Entity* Scene::createEntity(std::string tag) {
    Entity* entity = new Entity();
    entity->scene = this;
    entity->tag = tag;
    entity->iid = allObjects.size();
    tags[tag].push_back(entity);
    allObjects.push_back(entity);
    return entity;
}


std::vector<Entity*> Scene::getTag(std::string tag) {
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

    SDL_SetRenderDrawColor(GameManager::renderer, 0, 0, 0, 255);
    SDL_RenderClear(GameManager::renderer);

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
            Entity* entity = component->entity;

            if (entity->toDestroy) {
                component->onDestroy();
                it = clist->erase(it);

                std::remove(allObjects.begin(), allObjects.end(), entity);
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
            if (a->layer == b->layer) return a->entity->iid < b->entity->iid;
            return a->layer < b->layer;
            });

    for (Component* component : layeredComponents) {
        component->update(deltaTime);
    }

    //for (auto it = layeredComponents->begin(); it != layeredComponents->end();) {
    //    Component* component = *it;
    //    if (component == nullptr) continue;
    //    Entity* entity = component->entity;

    //    if (entity->toDestroy) {
    //        component->onDestroy();
    //        it = layeredComponents->erase(it);

    //        std::remove(allObjects.begin(), allObjects.end(), entity);
    //    } else {
    //        component->update(deltaTime);
    //        ++it;
    //    }
    //}

    //for (auto& c : components) {
    //    std::vector<Component*>* clist = &c.second;
}

std::vector<Entity*> Scene::getAllObjects() {
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
    //         Entity* entity = *it;
    //         if (entity) {
    //             delete entity;
    //             it = group->erase(it);
    //         } else {
    //             ++it;
    //         }
    //     }
    //     delete group;
    // }
    //groups.clear();
    //entitys.clear();
    //layers.clear();
}

void Scene::loadLDTK(std::string file) {
    std::ifstream f(file);
    json data = json::parse(f);

    auto& layerInstances = data["levels"][0]["layerInstances"];
    for (auto it = layerInstances.rbegin(); it != layerInstances.rend(); ++it) { 
        auto const& layer = *it;
        if (layer["__type"] == "Tiles") {
            int tileWidth = layer ["__gridSize"];
            int tileHeight = layer["__gridSize"];

            for (auto const& tile : layer["gridTiles"]) {
                Entity* tileObject = createEntity(layer["__identifier"]);

                tileObject->addComponent<Sprite>();

                std::string imageFileLocation;

                imageFileLocation.append("img/ldtk"); 
                imageFileLocation.append("/"); 
                imageFileLocation.append(layer["__tilesetRelPath"]);

                tileObject->getComponent<Sprite>().loadTexture(imageFileLocation);

                tileObject->getComponent<Sprite>().sourceRect.x = tile["src"][0];
                tileObject->getComponent<Sprite>().sourceRect.y = tile["src"][1];
                tileObject->getComponent<Sprite>().sourceRect.w = layer["__gridSize"];
                tileObject->getComponent<Sprite>().sourceRect.h = layer["__gridSize"];

                tileObject->box->setSize({layer["__gridSize"], layer["__gridSize"]});

                tileObject->box->setPosition({tile["px"][0], tile["px"][1]});
                tileObject->box->setSize({tileWidth, tileHeight});
            }
        }

        else if (layer["__type"] == "Entities") {
            for (auto const& entity : layer["entityInstances"]) {
                Entity* object = createEntity(entity["__identifier"]);

                object->addComponent<Sprite>();

                object->box->setPosition({entity["px"][0], entity["px"][1]});
                object->box->setSize({entity["width"], entity["height"]});
            }
        }
    }
}

void Scene::load() {}


