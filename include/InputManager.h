#pragma once

#include <SDL.h>
#include <unordered_map>
#include <SDL_gamecontroller.h>
#include <vector>
//#include "GameManager.h"
#include "Math.h"

// #define NUM_KEYS 256 // Total number of keys
#define NUM_KEYS 286 // Total number of keys

class InputManager {
public:
    InputManager();
    static void update();
    static bool checkInput(const std::string& input);
    static int checkVertical();
    static int checkHorizontal();
    static Vector2f checkAxis();
    static Vector2f getMouseWorldPosition();
    static Vector2f getMouseUIPosition();
    static Vector2f getMouseScreenPosition();

    static bool mousePressed;
    static bool jumpPressed;

    static bool keys[NUM_KEYS];
    static bool keysUped[NUM_KEYS];
};
