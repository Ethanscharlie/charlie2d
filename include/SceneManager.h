#pragma once

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "Entity.h"
#include <string>
#include <unordered_map>
#include "Scene.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Math.h"

class Scene;

class SceneManager {
public:
    SceneManager();
    static void init(Vector2f originalSize);
    static void AddScene(const std::string& name, Scene* scene);
    static void LoadScene(const std::string& name);
    static Scene* GetCurrentScene();
    static void Update();
    static void Render();
    static void playSound(std::string filename, bool loop =false);
    static void quit();

    static SDL_Window* window;
    static SDL_Renderer* renderer;
    static SDL_Texture* screenTexture;
    static int windowWidth;
    static int windowHeight;
    static int originalWidth;
    static int originalHeight;
    static float screen_change_scale;
    static bool running;
    static Vector2f camera;
    
private:
  static std::unordered_map<std::string, Scene*> scenes;
  static Scene* currentScene;
  static Scene* loadingScene;

  static int transition; // 0 -> Not Going, 1 -> Going, 2 -> Finished
  static Uint32 fadeStartTime;
  static int fade_time;
};
