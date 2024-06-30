#include "physicsBody.h"

void physicsBody::update(float deltaTime) {
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
  if (deltaTime > 0.2)
    std::cout << deltaTime << std::endl;
  if (deltaTime > 0.2)
    return;

  // Check for pointless move
  if (velocity.x == 0 && velocity.y == 0)
    return;

  std::vector<Box *> solids;
  for (SolidBody *col : GameManager::getComponents<SolidBody>()) {
    solids.push_back(&col->entity->box);
  }

  for (Tilemap *tilemap : GameManager::getComponents<Tilemap>()) {
    if (!tilemap->solid)
      continue;
    std::vector<Box> boxes = tilemap->tileGrid.getTileGroups();
    for (Box &box : boxes) {
      solids.push_back(&box);
    }
  }

  if (pushOut) {
    slideOut out = entity->box.slide(velocity * deltaTime, solids);

    if (out.horizontalHit)
      velocity.x = 0;
    if (out.verticleHit)
      velocity.y = 0;
  } else {
    entity->box.position += (velocity * deltaTime);
  }
}
