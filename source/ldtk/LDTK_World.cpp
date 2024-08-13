#include "ldtk/LDTK_World.hpp"
#include "ldtk/LDTK_FieldInstance.hpp"
#include "ldtk/LDTK_Level.hpp"
#include <memory>

namespace LDTK {
World::World(const json &levelsJson,
             std::map<int, LayerDefinition *> _layerDefinitions,
             std::map<int, EntityDefinition *> _entityDefinitions,
             std::map<int, Tileset *> _tilesets) {

  layerDefinitions = _layerDefinitions;
  entityDefinitions = _entityDefinitions;
  tilesets = _tilesets;

  for (const json &levelJson : levelsJson) {
    levels[levelJson["iid"]] = std::make_unique<Level>(
        levelJson, layerDefinitions, entityDefinitions, tilesets);
  }
}
} // namespace LDTK
