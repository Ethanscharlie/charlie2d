#ifndef SERIALIZER_H
#define SERIALIZER_H
#include "Component.h"
#include "Entity.h"
#include "GameManager.h"
#include "LDTKEntity.h"
#include "Vector2f.h"
#include <fstream>
#include <map>

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

extern std::map<int, Entity *> entityIidMap;
extern std::map<int, std::vector<int>> entityNeedsEntityMap;

template <typename T>
void deserializeBasicValue(json &propJson, PropertyData data) {
  json value = propJson;
  T *ptr = static_cast<T *>(data.value);
  *ptr = value;
}

json serialize(Entity *entity);
json serializeList(std::vector<Entity *> entities);

Entity *deserialize(json jsonData, bool start = true);
std::vector<Entity *> deserializeList(json jsonData, bool active = true);

void serializeAndWrite(Entity *entity, std::string filepath = "data.json");

std::vector<Entity *> loadCollection(std::filesystem::path path);

#endif
