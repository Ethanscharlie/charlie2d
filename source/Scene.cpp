#include "Scene.h"
#include "Entity.h"
#include "Component.h"
#include "EntityBox.h"

Scene::~Scene() {
    // destroy all entitys
    unload();
}

Scene::Scene()
{   
    SDL_GetRendererOutputSize(GameManager::renderer, &windowWidth, &windowHeight);
}

Entity* Scene::createEntity(std::string tag) {
    Entity* entity = new Entity();
    entity->box = new entityBox(entity);
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

    //if (deltaTime > 900) deltaTime = 0;

    lastTime = currentTime;
    lastTime = SDL_GetPerformanceCounter();

    //if (1.0f / deltaTime < 200) std::cout << "FPS: " << 1.0f / deltaTime << std::endl;

    SDL_SetRenderDrawColor(GameManager::renderer, 0, 0, 0, 255);
    SDL_RenderClear(GameManager::renderer);

    std::vector<Component*> layeredComponents;
    std::vector<Component*> toRemove;
    for (auto& c : components) {
        std::vector<Component*>* clist = &c.second;

        for (auto it = clist->begin(); it != clist->end();) {
            Component* component = *it;
            if (component == nullptr) continue;
            Entity* entity = component->entity;

              if (entity->toDestroy) {
                  for (Entity* e : entity->getChildren()) {
                        e->toDestroy = true;
                  }

                  component->onDestroy();
                  it = clist->erase(it);
                  allObjects.erase(std::remove(allObjects.begin(), allObjects.end(), entity), allObjects.end());

                  delete component;

                  // Delete Entity if there are no remaining components
                  if (entity->getComponents().size() <= 0) {
                    delete entity;
                  }
              }
              else if (entity->skipUpdate) {
                    entity->skipUpdate = false;
              }
              else if (!entity->active) { ++it;}
              else {
                  if (component->standardUpdate) {
                      if (component->useLayer) {
                          layeredComponents.push_back(component);
                      } else if (updateEntities) {
                          component->update(deltaTime);
                      }
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
}

std::vector<Entity*> Scene::getAllObjects() {
    return allObjects;
}

void Scene::unload()
{
    componentTypes.clear();
    components.clear();
    tags.clear();
}

