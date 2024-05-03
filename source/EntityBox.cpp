// #include "EntityBox.h"
// #include "Camera.h"
// #include "Entity.h"
// #include "GameManager.h"
//
// void entityBox::print() {
//   globalBox.print();
//   localBox.print();
// }
//
// void entityBox::updateGlobalBox() {
//   // globalBox.size = localBox.size;
//   //
//   // if (entity->getParent() == nullptr) {
//   //   globalBox.position = localBox.position;
//   // } else {
//   //   if (anchor == 0) // Top Left
//   //   {
//   //     globalBox.position =
//   //         localBox.position + entity->getParent()
//   //                                 ->require<entityBox>()
//   //                                 ->globalBox.getTopLeftCorner();
//   //
//   //   } else if (anchor == 1) // Top Middle
//   //   {
//   //     globalBox.position =
//   //         localBox.position +
//   //         entity->getParent()->require<entityBox>()->globalBox.getTopMiddle();
//   //   } else if (anchor == 2) { // Top Right
//   //     globalBox.position =
//   //         localBox.position + entity->getParent()
//   //                                 ->require<entityBox>()
//   //                                 ->globalBox.getTopRightCorner();
//   //   } else if (anchor == 3) { // Middle Left
//   //     globalBox.position =
//   //         localBox.position +
//   //         entity->getParent()->require<entityBox>()->globalBox.getMiddleLeft();
//   //   } else if (anchor == 4) // Middle
//   //   {
//   //     globalBox.position =
//   //         localBox.position +
//   //         entity->getParent()->require<entityBox>()->globalBox.getCenter();
//   //   } else if (anchor == 5) { // Middle Right
//   //     globalBox.position =
//   //         localBox.position +
//   //         entity->getParent()->require<entityBox>()->globalBox.getMiddleRight();
//   //   } else if (anchor == 6) { // Bottom Left
//   //     globalBox.position =
//   //         localBox.position + entity->getParent()
//   //                                 ->require<entityBox>()
//   //                                 ->globalBox.getBottomLeftCorner();
//   //   } else if (anchor == 7) // Botton Middle
//   //   {
//   //     globalBox.position =
//   //         localBox.position + entity->getParent()
//   //                                 ->require<entityBox>()
//   //                                 ->globalBox.getBottomMiddle();
//   //   } else if (anchor == 8) {
//   //     globalBox.position =
//   //         localBox.position + entity->getParent()
//   //                                 ->require<entityBox>()
//   //                                 ->globalBox.getBottomRightCorner();
//   //   }
//   //
//   //   for (Entity *child : entity->getChildren()) {
//   //     if (child == nullptr) {
//   //       std::cout << "Nullptr child" << std::endl;
//   //       continue;
//   //     }
//   //     child->require<entityBox>()->updateGlobalBox();
//   //   }
//   // }
// }
//
// void entityBox::updateLocalBox() {
//   // localBox.size = globalBox.size;
//   //
//   // if (entity->getParent() == nullptr) {
//   //   localBox.position = globalBox.position;
//   // } else {
//   //   if (anchor == 0) // Top Left
//   //   {
//   //     localBox.position =
//   //         globalBox.position - entity->getParent()
//   //                                  ->require<entityBox>()
//   //                                  ->globalBox.getTopLeftCorner();
//   //   } else if (anchor == 1) // Top Middle
//   //   {
//   //     localBox.position =
//   //         globalBox.position -
//   //         entity->getParent()->require<entityBox>()->globalBox.getTopMiddle();
//   //   } else if (anchor == 2) {
//   //     localBox.position =
//   //         globalBox.position - entity->getParent()
//   //                                  ->require<entityBox>()
//   //                                  ->globalBox.getTopRightCorner();
//   //   } else if (anchor == 3) {
//   //     localBox.position =
//   //         globalBox.position -
//   //         entity->getParent()->require<entityBox>()->globalBox.getMiddleLeft();
//   //   } else if (anchor == 4) // Middle
//   //   {
//   //     localBox.position =
//   //         globalBox.position -
//   //         entity->getParent()->require<entityBox>()->globalBox.getCenter();
//   //   } else if (anchor == 5) {
//   //     localBox.position =
//   //         globalBox.position -
//   //         entity->getParent()->require<entityBox>()->globalBox.getMiddleRight();
//   //
//   //   } else if (anchor == 6) {
//   //     localBox.position =
//   //         globalBox.position - entity->getParent()
//   //                                  ->require<entityBox>()
//   //                                  ->globalBox.getBottomLeftCorner();
//   //   } else if (anchor == 7) // Botton Middle
//   //   {
//   //     localBox.position =
//   //         globalBox.position - entity->getParent()
//   //                                  ->require<entityBox>()
//   //                                  ->globalBox.getBottomMiddle();
//   //   } else if (anchor == 8) {
//   //     localBox.position =
//   //         globalBox.position - entity->getParent()
//   //                                  ->require<entityBox>()
//   //                                  ->globalBox.getBottomRightCorner();
//   //   }
//   // }
//   //
//   // for (Entity *child : entity->getChildren()) {
//   //   if (child == nullptr) {
//   //     std::cout << "Nullptr child" << std::endl;
//   //     continue;
//   //   }
//   //   child->require<entityBox>()->updateGlobalBox();
//   // }
// }
//
// // Vector2f entityBox::getScreenPosition() {
// //   float scaler =
// //       GameManager::screen_change_scale * Camera::getScale();
// //       // ((GameManager::gameWindowSize.x + GameManager::gameWindowSize.y) /
// //       //  (GameManager::camera.size.x + GameManager::camera.size.y));
// //   Vector2f renderPos = entity->require<entityBox>()->getPosition() * scaler;
// //   return renderPos - (Camera::getPosition() * scaler -
// //                       GameManager::currentWindowSize / 2);
// // }
// //
// // Vector2f entityBox::getUIPosition() {
// //   return {GameManager::screen_change_scale *
// //   (entity->require<entityBox>()->getPosition().x) +
// //               GameManager::currentWindowSize.x / 2,
// //           GameManager::screen_change_scale *
// //           (entity->require<entityBox>()->getPosition().y) +
// //               GameManager::currentWindowSize.y / 2};
// // }
