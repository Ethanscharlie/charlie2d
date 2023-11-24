#pragma once

#include "Collider.h"
#include "Component.h"
#include "Entity.h"
#include "Math.h"

class physicsBody : public Component {
public:
  physicsBody() : Component("physicsBody"){};

  void start() override { entity->require<Collider>(); }

  void update(float deltaTime) override {
    // Adjusts velocity if greater then max
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
    
    slideOut out = entity->box->slide(velocity * deltaTime, scene->getTag("Ground"));
    // if (out.horizontalHit) velocity.x = 0;
    if (out.verticleHit) velocity.y = 0;
  }

  //   // Start
  //   Vector2f movement = velocity * deltaTime;
  //
  //   // Create Horizonal check box
  //   // The check box is sized to the move to prevent clipping
  //   // at high speeds and low framerates
  //   Box checkBox = entity->box->getBox();
  //   // checkBox.size.x += abs(movement.x);
  //   // if (movement.x < 0) {
  //   //   checkBox.position.x += movement.x;
  //   //
  //   // }
  //   entity->box->changePosition({movement.x, 0});
  //   checkBox = entity->box->getBox();
  //
  //   // Horizontal check and move
  //   for (auto *col : scene->getGroup<Collider>()) {
  //     if (!col->solid)
  //       continue;
  //     if (!checkBox.checkCollision(col->entity->box->getBox()))
  //       continue;
  //
  //     Entity *other = col->entity;
  //     if (movement.x > 0) {
  //       entity->box->setXPosition(other->box->getBox().getLeft() -
  //                                 entity->box->getSize().x);
  //       velocity.x = 0;
  //     } else if (movement.x < 0) {
  //       entity->box->setXPosition(other->box->getBox().getRight());
  //       velocity.x = 0;
  //     }
  //   }
  //
  //   // Create verticle check box
  //   checkBox = entity->box->getBox();
  //   // checkBox.size.y += abs(movement.y);
  //   // if (movement.y < 0) {
  //   //   checkBox.position.y += movement.y;
  //   // }
  //
  //   entity->box->changePosition({0, movement.y});
  //   checkBox = entity->box->getBox();
  //
  //   // Verticle collision check and move
  //   for (auto *col : scene->getGroup<Collider>()) {
  //     if (!col->solid)
  //       continue;
  //     if (!checkBox.checkCollision(col->entity->box->getBox()))
  //       continue;
  //
  //     Entity *other = col->entity;
  //     if (movement.y > 0) {
  //       entity->box->setYPosition(other->box->getBox().getTop() -
  //                                 entity->box->getSize().y);
  //       velocity.y = 0;
  //     } else if (movement.y < 0) {
  //       entity->box->setYPosition(other->box->getBox().getBottom());
  //       velocity.y = 0;
  //     }
  //   }
  // }

  Vector2f velocity;
  Vector2f maxVelocity = {-1, -1};
};
// #pragma once
//
// #include "Collider.h"
// #include "Component.h"
// #include "Entity.h"
// #include "Math.h"
//
// class physicsBody : public Component {
// public:
//   physicsBody() : Component("physicsBody"){};
//
//   void start() override { entity->require<Collider>(); }
//
//   void update(float deltaTime) override {
//     // Adjusts velocity if greater then max
//     if (maxVelocity.x != -1) {
//       if (abs(velocity.x) > abs(maxVelocity.x))
//         velocity.x = velocity.x / abs(velocity.x) * maxVelocity.x;
//     }
//     if (maxVelocity.y != -1) {
//       if (abs(velocity.y) > abs(maxVelocity.y))
//         velocity.y = velocity.y / abs(velocity.y) * maxVelocity.y;
//     }
//
//     // Prevents teleporting if people click of the window and
//     // make deltatime bigger than... idk something problably
//     if (deltaTime > 0.2)
//       std::cout << deltaTime << std::endl;
//     if (deltaTime > 0.2)
//       return;
//
//     // Check for pointless move
//     if (velocity.x == 0 && velocity.y == 0)
//       return;
//
//     // Start
//     Vector2f movement = velocity * deltaTime;
//
//     // Create Horizonal check box
//     // The check box is sized to the move to prevent clipping
//     // at high speeds and low framerates
//     Box checkBox = entity->box->getBox();
//     checkBox.size.x += abs(movement.x);
//     if (movement.x < 0) {
//       checkBox.position.x += movement.x;
//       // checkBox.position.x -= entity->box->getSize().x;
//     } else {
//       // checkBox.position.x += entity->box->getSize().x;
//     }
//
//     entity->box->changePosition({movement.x, 0});
//
//     // Horizontal check and move
//     Entity *horizontalHit = nullptr;
//     for (auto *col : scene->getGroup<Collider>()) {
//       if (!col->solid)
//         continue;
//       if (!checkBox.checkCollision(col->entity->box->getBox()))
//         continue;
//
//       Entity *other = col->entity;
//       if (horizontalHit == nullptr) {
//         horizontalHit = other;
//       } else {
//         if (abs(abs(entity->box->getPosition().x) -
//               abs(horizontalHit->box->getPosition().x)) >
//             abs(abs(entity->box->getPosition().x) -
//               abs(other->box->getPosition().x))) {
//         }
//       }
//     }
//
//     if (horizontalHit != nullptr) {
//       if (movement.x > 0) {
//         entity->box->setXPosition(horizontalHit->box->getBox().getLeft() -
//                                   entity->box->getSize().x);
//         velocity.x = 0;
//       } else if (movement.x < 0) {
//         entity->box->setXPosition(horizontalHit->box->getBox().getRight());
//         velocity.x = 0;
//       }
//     }
//
//     // Create verticle check box
//     checkBox = entity->box->getBox();
//     checkBox.size.y += abs(movement.y);
//     if (movement.y < 0) {
//       checkBox.position.y += movement.y;
//       // checkBox.position.y -= entity->box->getSize().y;
//     } else {
//       // checkBox.position.y += entity->box->getSize().y;
//     }
//
//     entity->box->changePosition({0, movement.y});
//
//     // Verticle collision check and move
//     Entity *verticleHit = nullptr;
//     for (auto *col : scene->getGroup<Collider>()) {
//       if (!col->solid)
//         continue;
//       if (!checkBox.checkCollision(col->entity->box->getBox()))
//         continue;
//
//       Entity *other = col->entity;
//       if (verticleHit == nullptr) {
//         verticleHit = other;
//       } else {
//         if (abs(abs(entity->box->getPosition().y) -
//                 abs(verticleHit->box->getPosition().y)) >
//             abs(abs(entity->box->getPosition().y) -
//                 abs(other->box->getPosition().y))) {
//         }
//       }
//     }
//
//     if (verticleHit != nullptr) {
//       if (movement.y > 0) {
//         entity->box->setYPosition(verticleHit->box->getBox().getTop() -
//                                   entity->box->getSize().y);
//         velocity.y = 0;
//       } else if (movement.y < 0) {
//         entity->box->setYPosition(verticleHit->box->getBox().getBottom());
//         velocity.y = 0;
//       }
//     }
//   }
//
//   Vector2f velocity;
//   Vector2f maxVelocity = {-1, -1};
// };
