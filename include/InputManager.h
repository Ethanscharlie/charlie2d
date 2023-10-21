#pragma once

#include <SDL.h>
#include <unordered_map>
#include <SDL_gamecontroller.h>
#include "GameManager.h"
#include "Math.h"


class InputManager {
public:
    InputManager();
    static void update();
    static bool checkInput(const std::string& input);
    static int checkVertical();
    static int checkHorizontal();
    static Vector2f checkAxis();
    static Vector2f getMousePosition();

    static bool mousePressed;
    static bool jumpPressed;
};
