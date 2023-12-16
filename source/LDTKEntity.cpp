#include "LDTKEntity.h"
#include "ldtk.h"

void LDTKEntity::onDestroy() {
  LDTK::entities.erase(
      std::remove(LDTK::entities.begin(), LDTK::entities.end(), entity),
      LDTK::entities.end());
}
