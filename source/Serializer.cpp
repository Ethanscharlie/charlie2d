#include "Serializer.h"
#include "Text.h"
#include <string>

json serialize(Entity *entity) {
  json jsonData;
  jsonData["tag"] = entity->tag;
  jsonData["x"] = entity->box->getPosition().x;
  jsonData["y"] = entity->box->getPosition().y;
  jsonData["w"] = entity->box->getSize().x;
  jsonData["h"] = entity->box->getSize().y;

  for (auto [type, component] : entity->components) {
    for (PropertyData data : component->propertyRegister) {
      json &prop =
          jsonData["Components"][getTypeNameWithoutNumbers(type)][data.name];

      if (data.type == typeid(float)) {
        prop = *static_cast<float *>(data.value);
      }

      else if (data.type == typeid(Vector2f)) {
        Vector2f &vector = *static_cast<Vector2f *>(data.value);
        prop["x"] = vector.x;
        prop["y"] = vector.y;
      }

      else if (data.type == typeid(Box)) {
        Box &box = *static_cast<Box *>(data.value);
        prop["x"] = box.position.x;
        prop["y"] = box.position.y;
        prop["w"] = box.size.x;
        prop["h"] = box.size.y;
      }

      else if (data.type == typeid(bool)) {
        prop = *static_cast<bool *>(data.value);
      }

      else if (data.type == typeid(Image)) {
        Image *image = static_cast<Image *>(data.value);
        prop = image->path;
      }

      else if (data.type == typeid(std::string)) {
        std::string *string = static_cast<std::string *>(data.value);
        prop = *string;
      }

      else if (data.type == typeid(Font)) {
        Font *font = static_cast<Font *>(data.value);
        prop["path"] = font->filepath;
        prop["size"] = font->size;
      }
    }
  }

  return jsonData;
}

Entity *deserialize(json jsonData) {
  std::string tag = jsonData["tag"];
  Entity *entity = GameManager::createEntity(tag);

  float x = jsonData["x"];
  float y = jsonData["y"];
  float w = jsonData["w"];
  float h = jsonData["h"];

  entity->box->setPosition(Vector2f(x, y));
  entity->box->setSize(Vector2f(w, h));

  for (auto [componentName, componentJson] : jsonData["Components"].items()) {
    Component *component =
        GameManager::componentRegistry[componentName](entity);
    for (PropertyData &data : component->propertyRegister) {
      std::cout << data.name << "\n";

      if (componentJson.find(data.name) == componentJson.end())
        continue;

      std::cout << "FIRE\n";

      json propJson = componentJson[data.name];
      if (data.type == typeid(float)) {
        float value = propJson;
        float *ptr = static_cast<float *>(data.value);
        *ptr = value;
      }

      else if (data.type == typeid(bool)) {
        bool value = propJson;
        bool *ptr = static_cast<bool *>(data.value);
        *ptr = value;
      }

      else if (data.type == typeid(Vector2f)) {
        Vector2f value = {propJson["x"], propJson["y"]};
        Vector2f *ptr = static_cast<Vector2f *>(data.value);
        *ptr = value;
      }

      else if (data.type == typeid(Box)) {
        Box value = {propJson["x"], propJson["y"], propJson["w"],
                     propJson["h"]};
        Box *ptr = static_cast<Box *>(data.value);
        *ptr = value;
      }

      else if (data.type == typeid(Image)) {
        Image *ptr = static_cast<Image *>(data.value);
        Image newImage = Image(propJson);
        *ptr = newImage;
      }

      else if (data.type == typeid(std::string)) {
        std::string *ptr = static_cast<std::string *>(data.value);
        *ptr = propJson;
      }

      else if (data.type == typeid(Font)) {
        Font *ptr = static_cast<Font *>(data.value);
        *ptr = Font(propJson["path"], propJson["size"]);
      }
    }
  }

  return entity;
}

void serializeAndWrite(Entity *entity, std::string filepath) {
  json jsonData = serialize(entity);

  std::ofstream file(filepath);
  file << std::setw(2) << jsonData << std::endl;
  file.close();
}
