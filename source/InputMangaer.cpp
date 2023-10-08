#include "InputManager.h"

bool InputManager::mousePressed = false;
bool InputManager::jumpPressed = false;

InputManager::InputManager()
{

}

void InputManager::update()
{
    mousePressed = false;
    jumpPressed = false;
}

bool InputManager::checkInput(const std::string& input)
{
    SDL_PumpEvents();
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
//    std::cout << keyboardState << std::endl;

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
    //const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

    if (checkInput("up")) {return 1;}
    if (checkInput("down")) {return -1;}
    return 0;
}

int InputManager::checkHorizontal()
{
    //const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

    if (checkInput("right")) {return 1;}
    if (checkInput("left")) {return -1;}
    return 0;
}

Vector2f InputManager::checkAxis() {
    return {checkHorizontal(), -checkVertical()};
}

Vector2f InputManager::getMousePosition() {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    return {mouseX / GameManager::screen_change_scale + GameManager::camera.position.x - GameManager::gameWindowSize.x/2,
           mouseY / GameManager::screen_change_scale + GameManager::camera.position.y - GameManager::gameWindowSize.y/2};
}
