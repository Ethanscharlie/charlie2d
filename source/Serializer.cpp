#include "Serializer.h"
#include "Entity.h"
#include "Light.h"
#include "ShadowFilter.h"
#include "Text.h"
#include "Tile.h"
#include <fstream>
#include <string>

std::map<int, Entity *> entityIidMap;

json serialize(Entity *entity) {
  if (entity->checkComponent<ShadowFilter>()) {
    if (entity->get<ShadowFilter>() == GameManager::shadowFilter) {
      return {};
    }
  }

  json jsonData;
  jsonData["tag"] = entity->tag;
  jsonData["x"] = entity->box.position.x;
  jsonData["y"] = entity->box.position.y;
  jsonData["w"] = entity->box.size.x;
  jsonData["h"] = entity->box.size.y;
  jsonData["iid"] = entity->iid;
  jsonData["name"] = entity->name;
  jsonData["group"] = entity->group;
  jsonData["layer"] = entity->layer;
  jsonData["renderType"] = static_cast<int>(entity->renderPositionType);

  for (auto [type, component] : entity->components) {
    jsonData["Components"][getTypeNameWithoutNumbers(type)];
    for (PropertyData data : component->propertyRegister) {
      json &prop =
          jsonData["Components"][getTypeNameWithoutNumbers(type)][data.name];

      if (data.type == typeid(float)) {
        prop = *static_cast<float *>(data.value);
      }

      else if (data.type == typeid(int)) {
        prop = *static_cast<int *>(data.value);
      }

      else if (data.type == typeid(Uint8)) {
        prop = *static_cast<Uint8 *>(data.value);
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

      else if (data.type == typeid(Entity *)) {
        Entity **entityPtr = static_cast<Entity **>(data.value);
        Entity *entity = *entityPtr;
        prop = 0;
        if (entity != nullptr) {
          prop = entity->iid;
        }
      }

      else if (data.type == typeid(TileGrid)) {
        TileGrid tileGrid = *static_cast<TileGrid *>(data.value);

        for (auto &[x, xlevel] : tileGrid.tiles) {
          for (auto &[y, tile] : xlevel) {
            json tileJson;
            tileJson["image"] = tile.image;
            tileJson["srcRectX"] = tile.srcRect.x;
            tileJson["srcRectY"] = tile.srcRect.y;
            tileJson["srcRectW"] = tile.srcRect.w;
            tileJson["srcRectH"] = tile.srcRect.h;
            tileJson["gridX"] = x;
            tileJson["gridY"] = y;

            prop["tiles"].push_back(tileJson);
          }
        }
      }
    }
  }

  return jsonData;
}

Entity *deserialize(json jsonData, bool start) {
  if (jsonData.is_null())
    return nullptr;

  std::string tag = jsonData["tag"];
  Entity *entity = GameManager::createEntity(tag);

  float x = jsonData["x"];
  float y = jsonData["y"];
  float w = jsonData["w"];
  float h = jsonData["h"];

  entity->box.position = (Vector2f(x, y));
  entity->box.size = (Vector2f(w, h));

  entity->iid = jsonData["iid"];
  entity->name = jsonData["name"];
  entity->group = jsonData["group"];
  entity->layer = jsonData["layer"];

  entity->renderPositionType =
      static_cast<EntityRenderPositionType>(jsonData["renderType"]);

  entityIidMap[entity->iid] = entity;

  for (auto [componentName, componentJson] : jsonData["Components"].items()) {
    Component *component =
        GameManager::componentRegistry[componentName](entity);
    for (PropertyData &data : component->propertyRegister) {
      std::cout << data.name << "\n";

      if (componentJson.find(data.name) == componentJson.end())
        continue;

      json propJson = componentJson[data.name];
      if (data.type == typeid(float)) {
        deserializeBasicValue<float>(propJson, data);
      }

      else if (data.type == typeid(int)) {
        deserializeBasicValue<int>(propJson, data);
      }

      else if (data.type == typeid(Uint8)) {
        deserializeBasicValue<Uint8>(propJson, data);
      }

      else if (data.type == typeid(bool)) {
        deserializeBasicValue<bool>(propJson, data);
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

      else if (data.type == typeid(Entity *)) {
        int iid = static_cast<int>(propJson);
        Entity **entityPtr = static_cast<Entity **>(data.value);
        Entity *entity = *entityPtr;
        *entityPtr = entityIidMap[iid];
      }

      else if (data.type == typeid(TileGrid)) {
        TileGrid *tileGrid = static_cast<TileGrid *>(data.value);

        for (json &tileJson : propJson["tiles"]) {
          GridTile tile;
          tile.image = tileJson["image"];
          tile.srcRect.x = tileJson["srcRectX"];
          tile.srcRect.y = tileJson["srcRectY"];
          tile.srcRect.w = tileJson["srcRectW"];
          tile.srcRect.h = tileJson["srcRectH"];

          tileGrid->setTile(tile, tileJson["gridX"], tileJson["gridY"]);
        }
      }
    }

    if (start)
      component->start();
  }

  if (!start) {
    for (auto &[type, component] : entity->components) {
      if (!component->typeIsRendering)
        component->standardUpdate = false;
    }
  }

  return entity;
}

json serializeList(std::vector<Entity *> entities) {
  json entitiesListJson;
  for (Entity *entity : entities) {
    entitiesListJson[entity->tag].push_back(serialize(entity));
  }
  return entitiesListJson;
}

std::vector<Entity *> deserializeList(json jsonData, bool active) {
  std::vector<Entity *> entities;
  for (json entityGroup : jsonData) {
    for (json entityJson : entityGroup) {
      Entity *dentity = deserialize(entityJson, active);
      if (dentity == nullptr)
        continue;

      entities.push_back(dentity);

      for (auto &[type, component] : dentity->components) {
        if (!component->typeIsRendering)
          component->standardUpdate = active;
      }
    }
  }
  return entities;
}

void serializeAndWrite(Entity *entity, std::string filepath) {
  json jsonData = serialize(entity);

  std::ofstream file(filepath);
  file << std::setw(2) << jsonData << std::endl;
  file.close();
}

std::vector<Entity *> loadCollection(std::filesystem::path path) {
  std::ifstream file(path);
  auto out = deserializeList(json::parse(file));
  file.close();
  return out;
}
