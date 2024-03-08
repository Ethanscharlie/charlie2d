@mainpage Project Overview
@include README.md
# charlie2d
A simple c++ w/SDL2 game engine

TODO:
Fix fade transitions
Try to fix imgui positioning
Fix scedules bad agrument organizaiton
Fix up documention and quickstart guide
Write up a full game guide

DOCS HAVE BEEN FINISHED HOLY HELL THAT TOOK SO LONG
# [DOCS](https://ethanscharlie.github.io/charlie2d/)

## Examples 

[ICNTBall](https://github.com/Ethanscharlie/ICNTBall/), a 2D metroidvania platformer

## Quickstart
#### The main fuction
```
int main()
{   
    GameManager::init({1920, 1080});

    GameManager::createEntity("Player");

    GameManager::doUpdateLoop();
    return 0;
}
```
This is what the main should look like, the init() must be ran first and will also set 
the 'original' resolution, note that this should be set once and left because changing it will change
game behavior

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
background->get<Sprite>()->loadTexture("img/background.png");

Entity* background2 = GameManager::createEntity("Background");
// Require adds or gets depending on if the entity has the component
background2->require<Sprite>()->loadTexture("img/background.png");
```

Components can be created an added (Kinda like scripts)
```
class Player : public Component {
    public:
    void start() override {
        entity->require<Sprite>()->loadTexture("img/player.png");
    }

    void update(float deltaTime) override {
        entity->get<Sprite>()->angle.lookAt(box->getBox().getCenter(), InputManager::getMousePosition());
    }
};
```
The start method will run during the component initalization
Update will of course run every game loop
And then can be added to Entities
```
Entity* player = createEntity("Player");
player->add<Player>();
```
