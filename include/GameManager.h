#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <functional>
#include <string>
#include <unordered_map>
#include "InputManager.h"
#include "ResourceManager.h"
#include "Math.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

class Scene;

class GameManager {
public:
    GameManager();
    static void init(Vector2f originalSize={1920,1080});
    static void AddScene(const std::string& name, Scene* scene);
    static void LoadScene(const std::string& name);
    static Scene* getCurrentScene();
    static void Update();
    static void Render();
    static void playSound(std::string filename, bool loop =false);
    static void quit();
    static void doUpdateLoop();

    static void doFade(std::function<void()> middle=[](){});

    static SDL_Window* window;
    static SDL_Renderer* renderer;
    static SDL_Texture* screenTexture;
    //static int windowWidth;
    //static int windowHeight;
    //static int originalWidth;
    //static int originalHeight;
    static Vector2f gameWindowSize;
    static Vector2f currentWindowSize;
    
    static float screen_change_scale;
    static bool running;

    static void setCamera(const Vector2f& position);
    static Box camera;
    //static float cameraZoom;
    static Box cameraLimitBox;
    static int transition; // 0 -> Not Going, 1 -> Going, 2 -> Finished
    
private:
  static std::unordered_map<std::string, Scene*> scenes;
  static Scene* currentScene;
  static Scene* loadingScene;

  static Uint32 fadeStartTime;
  static int fade_time;

  static std::function<void()> onMiddleFade ;
};

#endif
