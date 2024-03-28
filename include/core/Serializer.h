#ifndef SERIALIZER_H
#define SERIALIZER_H
#include "GameManager.h"
#include "LDTKEntity.h"
#include "Vector2f.h"
#include "Entity.h"
#include "EntityBox.h"
#include <fstream> 

#define GET_PROP(x)                                                            \
  { #x, &x }

template <typename T> bool registerComponentType(const std::string &typeName) {
  GameManager::componentRegistry[typeName] = [](Entity *entity) {
    return entity->add<T>(false);
  };

  return true;
}

#define REGISTER_COMPONENT_TYPE(Type)                                          \
  namespace {                                                                  \
  static bool Type##_registered = registerComponentType<Type>(#Type);          \
  }

// Can Handle:
// float
// Vector2f
// Box
// bool
// Image
json serialize(Entity *entity); 

// Can Handle:
// float
// Vector2f
// bool
// Image
Entity* deserialize(json jsonData);

void serializeAndWrite(Entity *entity, std::string filepath = "data.json");

#endif
