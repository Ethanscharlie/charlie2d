#include "Entity.hpp"

void Entity::remove(std::type_index type) {
  // Remove from GameManager
  Component *component = components[type];
  GameManager::removeComponent(component, type);

  // Remove from entity
  components.erase(type);

  delete component;

  std::cout << "Removed component " << type.name() << std::endl;
}

void Entity::update() {
  for (auto [type, component] : components) {
    if (!component->standardUpdate)
      continue;
    if (!GameManager::updateEntities && !component->typeIsRendering)
      continue;
    component->update(GameManager::deltaTime);
  }
}

void Entity::changeTag(std::string newTag) {
  GameManager::changeEntityTag(this, newTag);
}
