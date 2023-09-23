#pragma once
#include "Entity.h"

class Entity;

struct entityBox {
    entityBox(Entity* _entity);

    Vector2f getLocalPosition() {return localBox.position;}
    Vector2f getLocalSize()     {return localBox.size;}

    void setLocalPosition(const Vector2f& point) {localBox.position = point; updateGlobalBox();} 
    void setLocalXPosition(float x) {localBox.position.x = x; updateGlobalBox();}
    void setLocalYPosition(float y) {localBox.position.y = y; updateGlobalBox();}
    void changeLocalPosition(const Vector2f& change) {localBox.position += change; updateGlobalBox();}

    void setLocalSize(const Vector2f& point) {localBox.size = point; updateGlobalBox();} 
    void setLocalXSize(float x) {localBox.size.x = x; updateGlobalBox();}
    void setLocalYSize(float y) {localBox.size.y = y; updateGlobalBox();}
    void changeLocalSize(const Vector2f& change) {localBox.size += change; updateGlobalBox();}

    void setLocalWithCenter(const Vector2f& point) {localBox.setWithCenter(point); updateGlobalBox();}



    // GLOBAL POSITIONS
    Vector2f getPosition() {return globalBox.position;}
    Vector2f getSize()     {return globalBox.size;}

    void setPosition(const Vector2f& point) {globalBox.position = point; updateLocalBox();} 
    void setXPosition(float x) {globalBox.position.x = x; updateLocalBox();}
    void setYPosition(float y) {globalBox.position.y = y; updateLocalBox();}
    void changePosition(const Vector2f& change) {globalBox.position += change; updateLocalBox();}

    void setSize(const Vector2f& point) {globalBox.size = point; updateLocalBox();} 
    void setXSize(float x) {globalBox.size.x = x; updateLocalBox();}
    void setYSize(float y) {globalBox.size.y = y; updateLocalBox();}
    void changeSize(const Vector2f& change) {globalBox.size += change; updateLocalBox();}
    void setScale(const Vector2f& point) {Vector2f center = globalBox.getCenter(); setSize(point); setWithCenter(center);}
    void changeScale(const Vector2f& change) {Vector2f center = globalBox.getCenter(); changeSize(change); setWithCenter(center);}

    void setWithCenter(const Vector2f& point) {globalBox.setWithCenter(point); updateLocalBox();}


    Box getLocalBox() {return localBox;}
    Box getBox() {return globalBox;}

    void updateGlobalBox() ;
    void updateLocalBox() ;
    void print();

    Entity* entity = nullptr;

    int anchor = 0;

    private:
    Box globalBox;
    Box localBox;
};

