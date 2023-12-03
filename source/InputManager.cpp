#include "GameManager.h"
#include "InputManager.h"

bool InputManager::mousePressed = false;
bool InputManager::jumpPressed = false;

bool InputManager::keys[NUM_KEYS];
bool InputManager::keysUped[NUM_KEYS];

InputManager::InputManager()
{
  for (int i = 0; i < NUM_KEYS; ++i) {
    keys[i] = false; // Initialize all keys as released
    keysUped[i] = true;
  }
}

void InputManager::update()
{
    mousePressed = false;
    jumpPressed = false;

    for (int i = 0; i < NUM_KEYS; ++i) {
      keys[i] = false; // Initialize all keys as released
    }

}

bool InputManager::checkInput(const std::string& input)
{
    SDL_PumpEvents();
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
//    std::cout << keyboardState << std::endl;
    
    if (input == "any") {
        for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
            if (keyboardState[i]) {
                return true;
            }
        }

        return false;
    }
    if (input == "fire")
    {
        if(mousePressed){
            return true;
        }
    }
    else if (input == "jumpTrigger")
    {
        if(jumpPressed){
            return true;
        }
    }
    else if (input == "jump")
    {
        if (keyboardState[SDL_SCANCODE_SPACE])
        {
            return true;
        }
    }
    else if (input == "dash")
    {
        if (keyboardState[SDL_SCANCODE_LSHIFT] || keyboardState[SDL_SCANCODE_LSHIFT])
        {
            return true;
        }
    }
    else if (input == "rightBump")
    {
        if (keyboardState[SDL_SCANCODE_E])
        {
            return true;
        }
    }
    else if (input == "leftBump")
    {
        if (keyboardState[SDL_SCANCODE_Q])
        {
            return true;
        }
    }
    else if (input == "hit")
    {
        if (keyboardState[SDL_SCANCODE_F])
        {
            return true;
        }
    }
    else if (input == "take")
    {
        if (keyboardState[SDL_SCANCODE_E])
        {
            return true;
        }
    }
    else if (input == "up")
    {
        if (keyboardState[SDL_SCANCODE_W] or keyboardState[SDL_SCANCODE_UP])
        {
            return true;
        }
    }
    else if (input == "down")
    {
        if (keyboardState[SDL_SCANCODE_S] or keyboardState[SDL_SCANCODE_DOWN])
        {
            return true;
        }
    }
    else if (input == "right")
    {
        if (keyboardState[SDL_SCANCODE_D] or keyboardState[SDL_SCANCODE_RIGHT])
        {
            return true;
        }
    }
    else if (input == "left")
    {
        if (keyboardState[SDL_SCANCODE_A] or keyboardState[SDL_SCANCODE_LEFT])
        {
            return true;
        }
    }

    return false;
}

int InputManager::checkVertical()
{
    if (checkInput("up")) return 1;
    if (checkInput("down")) return -1;
    return 0;
}

int InputManager::checkHorizontal()
{
    if (checkInput("right")) return 1;
    if (checkInput("left")) return -1;
    return 0;
}

Vector2f InputManager::checkAxis() {
    return {static_cast<float>(checkHorizontal()), static_cast<float>(-checkVertical())};
}

Vector2f InputManager::getMouseWorldPosition() {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    Vector2f mouse = {static_cast<float>(mouseX), static_cast<float>(mouseY)};

    // float scaler = GameManager::screen_change_scale * ((GameManager::gameWindowSize.x +              
    //             GameManager::gameWindowSize.y) / (GameManager::camera.size.x + GameManager::camera.size.y));         
    // return (mouse - GameManager::currentWindowSize / 2) / scaler + GameManager::camera.getCenter();
    return {0,0};
}

Vector2f InputManager::getMouseUIPosition() {
    //return {GameManager::screen_change_scale * (entity->box->getPosition().x) + GameManager::currentWindowSize.x/2, 
    //GameManager::screen_change_scale * (entity->box->getPosition().y) + GameManager::currentWindowSize.y/2};    
    Vector2f mouse = getMouseScreenPosition();    
     return { (mouse.x - GameManager::currentWindowSize.x/2) / GameManager::screen_change_scale,                         
        (mouse.y - GameManager::currentWindowSize.y/2) / GameManager::screen_change_scale};                                 


}

Vector2f InputManager::getMouseScreenPosition() {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    return {static_cast<float>(mouseX), static_cast<float>(mouseY)};
}
