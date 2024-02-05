#include "Component.h"
#include "Entity.h"
#include "EntityBox.h"

class ExtendedComponent : public Component {
    public:
    void additionalStart() 
    {
        box = entity->add<entityBox>();
    }
    void start() override { additionalStart(); }
    void update(float deltaTime) override { update(); }
    virtual void update() {}
    
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
    
    entityBox *box = nullptr;
};