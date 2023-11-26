#ifndef SCENE_H
#define SCENE_H

#include "GameManager.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <map>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "Component.h"
// #include "Entity.h"

typedef std::vector<Component *> Group;

class Entity;

/**
 * \brief Made for inheritence (With the load method), handles the update loop
 * and can be added and loaded inside the GameManager static class
 */
class Scene {
public:
  virtual ~Scene();
  Scene();
  // void add(Entity* entity, const std::string& group);

  /**
   * \brief REMOVED DONT USE
   */
  template <typename C> void add(Component *component) {

    // if (components.count(typeid(C)) == 0) { //&&
    // std::find(componentTypes.begin(), componentTypes.end(), typeid(C)) ==
    // componentTypes.end()) {
    //     componentTypes.push_back(typeid(C));
    //     std::cout << "added type " << component->title << std::endl;
    // }

    // component->index = 0;//components[typeid(C)].size();
    // components[typeid(C)].push_back(component);
  }

  /**
   * \brief Gets Components via their type
   * \param tag Will only grab Components with entities of a certain tag,
   * Defaults to "" which ignores this Ex `getGroup<Sprite>()` Will get every
   * sprite component
   */
  template <typename C> std::vector<C *> getGroup(std::string tag = "") {
    std::vector<C *> hits;
    auto it = components.find(typeid(C));

    if (it != components.end()) {
      for (auto component : it->second) {
        // Static cast each Component* to C*
        if (component->entityTag == tag || tag == "") {
          hits.push_back(static_cast<C *>(component));
        }
      }
    } else
      components[typeid(C)];
    // throw std::runtime_error("Component group not found!");

    return hits;
  }

  /**
   * \brief Gets entitys via their tags
   * \param tag string for the tag
   * \return An std::vector of entitys
   */
  std::vector<Entity *> getTag(std::string tag);

  /**
   * \brief Creates and Entity
   * \param tag Currently the only way to set an Entitys tag
   * \return The newly made Entity
   */
  Entity *createEntity(std::string tag = "");
  /**
   * \brief A Vector of very entity in the scene
   */
  std::vector<Entity *> getAllObjects();
  void update();
  /**
   * \brief Code to run when the scene is loaded, Remember this class in made
   * for inheritence
   */
  virtual void load() = 0;
  /**
   * \brief Removes and deletes all Components and Entitys
   */
  void unload();

  /**
   * \brief Pause mode
   */
  bool updateEntities = true;
  float deltaTime = 0;

  // private:
  Uint64 lastTime;
  /**
   * \brief All Entitys in scene (sorted via tag)
   */
  std::unordered_map<std::string, std::vector<Entity *>> tags;
  std::vector<Entity *> allObjects;
  /**
   * \brief All Components in scene (sorted via type)
   */
  std::map<std::type_index, Group> components;
  // std::vector<std::string> layers;
  // std::vector<Entity*> entitys;
};

#endif
