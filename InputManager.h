#pragma once

#include <SDL.h>
#include <unordered_map>
#include <SDL_gamecontroller.h>
#include "SceneManager.h"


class InputManager {
public:
    InputManager();
    static void update();
    static bool checkInput(const std::string& input);
    static int checkVertical();
    static int checkHorizontal();

    static bool mousePressed;
};
