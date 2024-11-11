#include "Entity.hpp"
#include "GameManager.hpp"

void Entity::remove(std::type_index type) {
  // Remove from GameManager
  auto &component = components[type];

  // Remove from entity
  components.erase(type);

  std::cout << "Removed component " << type.name() << std::endl;
}

void Entity::update() {
  for (auto &[type, component] : components) {
    if (!component->standardUpdate)
      continue;
    component->update();
  }
}

void Entity::destroy() { toDestroy = true; }

bool Entity::isQueuedForDestruction() { return toDestroy; }

void Entity::setLayer(const int &layer) {
  this->layer = layer;  
  GameManager::resortEntitiesNextFrame();
}

int Entity::getLayer() { return layer; }
