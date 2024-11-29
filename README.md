@mainpage Project Overview
@include README.md
# charlie2d
A simple c++ w/SDL2 game engine

Please download to look at docs

### Some stuff I need to do
* Clean up the entity class
* Add music support
* Make audio contexts work in the browser correctly
* Make ldtk easier to access (Creating a unique pointer with it is a pain in the ass)
* Change getComponents to crawl through entites instead of saving a Components map in GameManager
* Make entities and components stored with unique pointers instead of using new and delete
* Stop passing entites and components with pointers (Using references instead). The plan is the store the entities in a vector with the references in a map of vectors by tag
* Add proper documentation for the events in InputManager
* Fix JumpMan to work with controller
* Remove deltaTime from the component update function
* Add a way to have event listeners automatially delete when an associated entity is destroyed
* Add a way to make images white flash (an image where every valid pixil is white)

## Setup
To make things easier I have created a project template that should 
make it as easy as possible to compile projects for any platform
https://github.com/Ethanscharlie/Charlie2D-Project-Template

## Quickstart
### Init
```
int main()
{   
    GameManager::init({1920, 1080});

    // Game code

    GameManager::doUpdateLoop();
    return 0;
}
```
The init() must be ran first and will also set the 'original' resolution, 
note that this should be set once and left because changing it will change
game behavior, defaults to 1920x1080 if no argument is given.

### Entities
```
Entity* player = GameManager::createEntity("Player");
```
Entites can be created with the GameManager::createEntity method.
In the instance above the Entity is given the tag "Player".

To get a list of entities you can use the GameManager::getEntities function
```
std::vector<Entity*> enemies = GameManager::getEntities("Enemy");
```
The same can be done for components
```
std::vector<Sprite*> spriteComponents = GameManager::getComponents<Sprite>();
```

### Components
```
Entity* background = GameManager::createEntity("Background");
background->add<Sprite>();
background->get<Sprite>()->loadTexture("res/background.png");

Entity* background2 = GameManager::createEntity("Background");
```

Components can be created an added (Kinda like scripts)
```
class Player : public Component {
    public:
    void start() override {

    }

    void update(float deltaTime) override {
        Sprite* sprite = entity->get<Sprite>()
        sprite->angle.lookAt(entity->box.getCenter(), InputManager::getMouseWorldPosition());
    }
};
```
The start method will run during the component initalization
Update will of course run every game loop
And then can be added to Entities
```
Entity* player = createEntity("Player");
player->add<Sprite>();
player->add<Player>();
```

### Events
Events can be called in one place and then recived by multiple other places
```
Event::addEventListener("LeftMouseButtonDown", []() {
    Audio("res/rickroll.mp3").play();
});
```
```
Event::fireEvent("PlayerHasDied");
```
Note: If an event is tied to an entity then remember to REMOVE it after the entity is DESTROIED
```
int id = Event::addEventListener("PlayerHasDied", [entity]() {
    entity->box.setWithCenter({0, 0});
});

entity->toDestroy = true;

Event::removeEventListener(id);
```
