#include "Entity.hpp"

void Entity::remove(std::type_index type) {
  // Remove from GameManager
  auto &component = components[type];

  // Remove from entity
  components.erase(type);

  std::cout << "Removed component " << type.name() << std::endl;
}

void Entity::update() {
  for (auto& [type, component] : components) {
    if (!component->standardUpdate)
      continue;
    component->update();
  }
}

void Entity::destroy() {
  toDestroy = true;
}
