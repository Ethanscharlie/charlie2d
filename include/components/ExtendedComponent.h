#pragma once
#include "Component.h"
#include "Entity.h"
#include "EntityBox.h"

class ExtendedComponent : public Component {
    public:
    virtual void start() override { }
    void update(float deltaTime) override { update(); }
    virtual void update() {}
    virtual void onDestroy() override {}
    
    template <typename C> 
    C *add() { 
        return entity->add<C>();
    }

    template <typename C> 
    C *get() { 
        return entity->get<C>();
    }

    template <typename C> 
    bool checkComponent() { 
        return entity->checkComponent<C>();
    }
};
