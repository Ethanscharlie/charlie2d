@mainpage Project Overview
@include README.md
# charlie2d
A simple c++ w/SDL2 game engine

Please download to look at docs

## Setup
To make things easier I have created a project template that should 
make it as easy as possible to compile projects for any platform
https://github.com/Ethanscharlie/Charlie2D-Project-Template

## Quickstart
#### Init
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

#### Entities
```
Entity* player = GameManager::createEntity("Player");
```
Entites can be created with the GameManager::createEntity method.
In the instance above the Entity is given the tag "Player".

#### Components
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
