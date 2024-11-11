#include "physicsBody.hpp"
#include "GameManager.hpp"
#include "ldtk/LDTK_Tilemap.hpp"

void physicsBody::update() {
  // Adjusts velocity if greater then max (Ignores if -1)
  if (maxVelocity.x != -1) {
    if (abs(velocity.x) > abs(maxVelocity.x))
      velocity.x = velocity.x / abs(velocity.x) * maxVelocity.x;
  }
  if (maxVelocity.y != -1) {
    if (abs(velocity.y) > abs(maxVelocity.y))
      velocity.y = velocity.y / abs(velocity.y) * maxVelocity.y;
  }

  // Prevents teleporting if people click of the window and
  // make deltatime bigger than... idk something problably
  if (GameManager::getDeltaTime() > 0.2)
    std::cout << GameManager::getDeltaTime() << std::endl;
  if (GameManager::getDeltaTime() > 0.2)
    return;

  // Check for pointless move
  if (velocity.x == 0 && velocity.y == 0)
    return;

  std::vector<Box *> solids;
  for (SolidBody *col : GameManager::getComponents<SolidBody>()) {
    solids.push_back(&col->entity.box);
  }

  for (LDTK::Tilemap *tilemap : GameManager::getComponents<LDTK::Tilemap>()) {
    if (!tilemap->layer)
      continue;
    if (!tilemap->solid)
      continue;
    for (Box &box : tilemap->layer->boxes) {
      solids.push_back(&box);
    }
  }

  if (pushOut) {
    slideOut out = entity.box.slide(velocity * GameManager::getDeltaTime(), solids);

    if (out.horizontalHit)
      velocity.x = 0;
    if (out.verticleHit)
      velocity.y = 0;
  } else {
    entity.box.position += (velocity * GameManager::getDeltaTime());
  }
}
