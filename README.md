@mainpage Project Overview
@include README.md
# charlie2d
A simple c++ w/SDL2 game engine

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
    GameManager::AddScene("game", new gameScene());
    GameManager::AddScene("menu", new menuScene());
    GameManager::LoadScene("menu");
    GameManager::doUpdateLoop();
    return 0;
}
```
This is what the main should look like, the init() must be ran first and will also set 
the 'original' resolution, note that this should be set once and left because changing it will change
game behavior

#### Scenes
```
class menuScene : public Scene {
    void load() override {

    }
};
```
Scenes are the containers for every entity, and can be defined as above
```
GameManager::AddScene("menu", new MenuScene());
GameManager::LoadScene("menu");
```
To be loaded they must be added to the scene map in the GameManager.
And then loaded with the string name

#### Entities
```
class GameScene : public Scene
{
    void load() override
    {
        Entity* player = createEntity("Player");
    }
};
```
Entites can be created with the createEntity method (inside of scenes).
In the instance above the Entity is given the tag "Player".

#### Components
```
class GameScene : public Scene
{
    void load() override
    {
        Entity* background = createEntity("Background");
        background->addComponent<Sprite>();
        background->getComponent<Sprite>().loadTexture("img/background.png");
    }
};
```

Components can be created an added (Kinda like scripts)
```
class Player : public Component {
    public:
    Player() : Component("Player") {}

    void start() override {
        entity->require<Sprite>(); // Adds or gets based on if the component has already been added to the entity
        entity->get<Sprite>()->loadTexture("img/player.png");
    }

    void update(float deltaTime) override {
        get<Sprite>()->angle.lookAt(box->getBox().getCenter(), InputManager::getMousePosition());
    }
};
```
`Player() : Component("Player") {}` This will set the title of the Components
The start method will run during the component initalization
Update will of course run every game loop
And then can be added to Entities
```
Entity* player = createEntity("Player");
player->add<Player>();
```
