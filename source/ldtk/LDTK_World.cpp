#include "ldtk/LDTK_World.hpp"
#include "ldtk/LDTK_FieldInstance.hpp"
#include "ldtk/LDTK_Level.hpp"
#include "ldtk/LDTK_Project.hpp"
#include <format>
#include <memory>
#include <stdexcept>

namespace LDTK {
World::World(const json &levelsJson, Project *_project) {

  project = _project;

  for (const json &levelJson : levelsJson) {
    int uid = levelJson["uid"];
    levels[uid] = std::make_unique<Level>(levelJson, project);
  }
}

Level *World::getLevelWithIID(std::string iid) {
  for (auto &[uid, level] : levels) {
    if (level->iid == iid) {
      return level.get();
    }
  }

  throw std::runtime_error(std::format("Level {} was not found", iid));
}
} // namespace LDTK
