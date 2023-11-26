#ifndef ENTITY_BOX_H
#define ENTITY_BOX_H

#include "Entity.h"
#include "Math.h"
#include <vector>

class Entity;

/**
 * \brief An output for the entityBox slide method
 */
struct slideOut {
  std::vector<Entity *> hitList;
  bool horizontalHit = true;
  bool verticleHit = false;
};

/**
 * \brief entityBoxs are a class for local and global positioning relitive to a
 * parent If no parent is set than you can ignore tho local part
 *
 * Every entity
 * has an entityBox (Think of these like Unity Transforms) These will be
 *
 * Changes either local or global box with change the oppite box as well to fix
 * with your changes
 *
 * This will be switched to a Component at some because because that makes more
 * sense
 */
struct entityBox {
  entityBox(Entity *_entity);
  ~entityBox() {}

  /**
   * \brief Gets the position of the local box
   */
  Vector2f getLocalPosition() { return localBox.position; }
  /**
   * \brief Gets the size of the local box
   */
  Vector2f getLocalSize() { return localBox.size; }

  /**
   * \brief Sets the position of the local box
   */
  void setLocalPosition(const Vector2f &point) {
    localBox.position = point;
    updateGlobalBox();
  }
  /**
   * \brief Sets the X position of the local box
   */
  void setLocalXPosition(float x) {
    localBox.position.x = x;
    updateGlobalBox();
  }
  /**
   * \brief Sets the Y position of the local box
   */
  void setLocalYPosition(float y) {
    localBox.position.y = y;
    updateGlobalBox();
  }
  /**
   * \brief Changes (Adds) the position of the local box
   */
  void changeLocalPosition(const Vector2f &change) {
    localBox.position += change;
    updateGlobalBox();
  }

  /**
   * \brief Sets the size of the local box
   */
  void setLocalSize(const Vector2f &point) {
    localBox.size = point;
    updateGlobalBox();
  }
  /**
   * \brief Sets the Width of the local box
   */
  void setLocalXSize(float x) {
    localBox.size.x = x;
    updateGlobalBox();
  }
  /**
   * \brief Sets the Height of the local box
   */
  void setLocalYSize(float y) {
    localBox.size.y = y;
    updateGlobalBox();
  }
  /**
   * \brief changes (Adds) the size of the local box
   */
  void changeLocalSize(const Vector2f &change) {
    localBox.size += change;
    updateGlobalBox();
  }

  /**
   * \brief Sets the center postion to the point of the localbox
   */
  void setLocalWithCenter(const Vector2f &point) {
    localBox.setWithCenter(point);
    updateGlobalBox();
  }

  // GLOBAL POSITIONS
  /**
   * \brief Gets the position of the global box
   */
  Vector2f getPosition() { return globalBox.position; }
  /**
   * \brief Gets the size of the global box
   */
  Vector2f getSize() { return globalBox.size; }

  /**
   * \brief Sets the position of the global box
   */
  void setPosition(const Vector2f &point) {
    globalBox.position = point;
    updateLocalBox();
  }
  /**
   * \brief Sets the X position of the global box
   */
  void setXPosition(float x) {
    globalBox.position.x = x;
    updateLocalBox();
  }
  /**
   * \brief Sets the Y position of the global box
   */
  void setYPosition(float y) {
    globalBox.position.y = y;
    updateLocalBox();
  }
  /**
   * \brief Changes (Adds) the position of the global box
   */
  void changePosition(const Vector2f &change) {
    globalBox.position += change;
    updateLocalBox();
  }
  /**
   * \brief Sets the size of the global box
   */

  void setSize(const Vector2f &point) {
    globalBox.size = point;
    updateLocalBox();
  }
  /**
   * \brief Sets the Width of the global box
   */
  void setXSize(float x) {
    globalBox.size.x = x;
    updateLocalBox();
  }
  /**
   * \brief Sets the Height of the global box
   */
  void setYSize(float y) {
    globalBox.size.y = y;
    updateLocalBox();
  }
  /**
   * \brief changes (Adds) the size of the global box
   */
  void changeSize(const Vector2f &change) {
    globalBox.size += change;
    updateLocalBox();
  }
  /**
   * \brief Sets the size of global box with keep the center in the same place
   */
  void setScale(const Vector2f &point) {
    Vector2f center = globalBox.getCenter();
    setSize(point);
    setWithCenter(center);
  }
  /**
   * \brief changes (Adds) the size of the global box
   */
  void changeScale(const Vector2f &change) {
    Vector2f center = globalBox.getCenter();
    changeSize(change);
    setWithCenter(center);
  }
  /**
   * \brief Sets the center postion to the point of the global box
   */

  void setWithCenter(const Vector2f &point) {
    globalBox.setWithCenter(point);
    updateLocalBox();
  }

  /**
   * \brief Gets a copy the raw local box
   */
  Box getLocalBox() { return localBox; }
  /**
   * \brief Gets a copy the raw global box
   */
  Box getBox() { return globalBox; }

  /**
   * \brief Gets the Screen position (Used for rendering)
   */
  Vector2f getScreenPosition();
  /**
   * \brief Gets the UI position (Used for UI rendering)
   */
  Vector2f getUIPosition();

  /**
   * \brief Called automaticaly, upadates when local box is changed
   */
  void updateGlobalBox();
  /**
   * \brief Called automaticaly, upadates when global box is changed
   */
  void updateLocalBox();
  /**
   * \brief Outputs the global box
   */
  void print();

  /**
   * \brief Moves the global box while using entites as walls that it can't pass
   * through \param move the change (You may want to multiply this by deltaTime)
   * \param solids Entitys that this object can't go through and will be pushed
   * out of
   * \param pushOut When false it won't use solids as walls and allows pass
   * through \param useMoveBox Used for continueing collsion detection
   * (Currently buggy, don't use with pushOut being true)
   * \return A slideOut which contains data about what occured during the slide
   */
  slideOut slide(const Vector2f &move, const std::vector<Entity *> &solids,
                 bool pushOut = true, bool useMoveBox = true) {
    slideOut out;

    // Start
    Vector2f movement = move;

    // Create Horizonal check box
    // The check box is sized to the move to prevent clipping
    // at high speeds and low framerates
    changePosition({movement.x, 0});
    if (!useMoveBox)
      getBox() = getBox();

    // Horizontal check and move
  HORREDO:
    for (Entity *col : solids) {
      if (!getBox().checkCollision(col->box->getBox()))
        continue;

      out.hitList.push_back(col);
      if (!pushOut)
        continue;

      Entity *other = col;
      if (movement.x > 0) {
        setXPosition(other->box->getBox().getLeft() - getSize().x);
        out.horizontalHit = true;
      } else if (movement.x < 0) {
        setXPosition(other->box->getBox().getRight());
        out.horizontalHit = true;
      }

      goto HORREDO;
    }

    // Create verticle check box
    changePosition({0, movement.y});
    if (!useMoveBox)
      getBox() = getBox();

    // Verticle collision check and move
  VERREDO:
    for (Entity *col : solids) {
      if (!getBox().checkCollision(col->box->getBox()))
        continue;

      out.hitList.push_back(col);
      if (!pushOut)
        continue;

      Entity *other = col;
      if (movement.y > 0) {
        setYPosition(other->box->getBox().getTop() - getSize().y);
        out.verticleHit = true;
      } else if (movement.y < 0) {
        setYPosition(other->box->getBox().getBottom());
        out.verticleHit = true;
      }

      goto VERREDO;
    }

    return out;
  }

  Entity *entity = nullptr;

  int anchor = 0;

private:
  Box globalBox;
  Box localBox;
};

#endif
