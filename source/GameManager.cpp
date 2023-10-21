#include "GameManager.h"
#include "Entity.h"
#include "Scene.h"

int GameManager::transition = 0; // 0 -> Not Going, 1 -> Going, 2 -> Finished
Uint32 GameManager::fadeStartTime = 0; 
int GameManager::fade_time = 300;

bool GameManager::running = true;

std::unordered_map<std::string, Scene*> GameManager::scenes;
Scene* GameManager::currentScene = nullptr;
Scene* GameManager::loadingScene = nullptr;

// int GameManager::originalWidth = 0;
// int GameManager::originalHeight = 0;
// int GameManager::windowWidth = 0;
// int GameManager::windowHeight = 0;
Vector2f GameManager::gameWindowSize = {0, 0};
Vector2f GameManager::currentWindowSize = {0, 0};
                                    
float GameManager::screen_change_scale = 0;
                                    
Box GameManager::camera = {0, 0, 0, 0};
Box GameManager::cameraLimitBox = {0, 0, 0, 0};
                                    
SDL_Window* GameManager::window = nullptr;
SDL_Renderer* GameManager::renderer = nullptr;
std::function<void()> GameManager::onMiddleFade = [](){};

GameManager::GameManager() {
    
}

void GameManager::init(Vector2f windowSize)
{
  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO); 
  SDL_Init(SDL_INIT_AUDIO); 
  TTF_Init();
  IMG_Init(IMG_INIT_PNG);
  SDL_SetWindowResizable(window, SDL_TRUE);

  //originalWidth = originalSize.x;
  //originalHeight = originalSize.y;
  //windowWidth =  GameManager::originalWidth  ;
  //windowHeight = GameManager::originalHeight ;
  gameWindowSize = windowSize;
  currentWindowSize = windowSize;
  camera.size = windowSize; 

  screen_change_scale = ((float) currentWindowSize.x + (float) currentWindowSize.y)
      / (gameWindowSize.x + gameWindowSize.y);


  window = SDL_CreateWindow("SDL2 works!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, currentWindowSize.x, currentWindowSize.y, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void GameManager::AddScene(const std::string& name, Scene* scene) {
  scenes[name] = scene;
}

void GameManager::LoadScene(const std::string& name) {
  Scene* scene = scenes[name];
  if (scene) {
    loadingScene = scene;
    if (!transition) {
        doFade([](){
                if (currentScene) {
                currentScene->unload();
                }

                loadingScene->load();
                currentScene = loadingScene;
        });
        //transition = 1;
        //fadeStartTime = SDL_GetTicks();
    }
  }
}

Scene* GameManager::getCurrentScene() {
    return currentScene;
}


void GameManager::Update() {
  InputManager::update();
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
      if (event.type == SDL_QUIT)
      {
          quit();
      }
      else if (event.type == SDL_MOUSEBUTTONDOWN)
      {
          InputManager::mousePressed = true;
      }
      else if (event.type == SDL_WINDOWEVENT) 
      {
          switch (event.window.event)
          {
              case SDL_WINDOWEVENT_RESIZED:
                  currentWindowSize.x = event.window.data1;
                  currentWindowSize.y = event.window.data2;

                  screen_change_scale = ((float) currentWindowSize.x + (float) currentWindowSize.y)
                      / (gameWindowSize.x + gameWindowSize.y);

                  break;
          }
          break;
      }
      else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym) {
                case SDLK_SPACE:
                    InputManager::jumpPressed = true;
            }
        }
  }

  if (currentScene) {
    currentScene->update();
  }

  if (transition == 1)
  {
      // Calculate the elapsed time since the start of the transition
      Uint32 elapsedTime = SDL_GetTicks() - fadeStartTime;

      SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, static_cast<Uint8>(255 * elapsedTime / fade_time));

      SDL_Rect fill_dst;
      fill_dst.w=currentWindowSize.x;
      fill_dst.h=currentWindowSize.y;
      fill_dst.x=0;
      fill_dst.y=0;

      SDL_RenderFillRect(GameManager::renderer, &fill_dst); 

      // Check if the fade transition is complete
      if (elapsedTime >= fade_time) {
          transition = 2;
        fadeStartTime = SDL_GetTicks();
        
        // ON MIDDLE
        //if (currentScene) {
        //  currentScene->unload();
        //}

        //loadingScene->load();
        //currentScene = loadingScene;
        onMiddleFade();
      }
  }

  if (transition == 2)
  {
      // Calculate the elapsed time since the start of the transition
      Uint32 elapsedTime = SDL_GetTicks() - fadeStartTime;

      SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, static_cast<Uint8>(255 - 255 * elapsedTime / fade_time));

      SDL_Rect fill_dst;
      fill_dst.w=currentWindowSize.x;
      fill_dst.h=currentWindowSize.y;
      fill_dst.x=0;
      fill_dst.y=0;

      SDL_RenderFillRect(renderer, &fill_dst); 

      // Check if the fade transition is complete
      if (elapsedTime >= fade_time) {
          transition = 0;
      }
  }

  SDL_RenderPresent(renderer);
}

void GameManager::doUpdateLoop() {
    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(Update, 0, 1);
    #else
    while (running) {
        Update();
    }
    #endif
}

void GameManager::Render() {
  if (currentScene) {
    // currentScene->Draw();
  }
}

void GameManager::playSound(std::string filename, bool loop) {
    //   Mix_Chunk* sound = ResourceManager::getInstance(renderer).getAudio(filename);

    //   int channel = Mix_PlayChannel(-1, sound, 0);
    //   if (loop) {
//  //         channel = Mix_PlayChannel(-1, sound, -1);
    //   }

    //   if (channel == -1) {
    //       printf("Failed to play sound effect: %s\n", Mix_GetError());
    //   }


    //   //Mix_FreeChunk(sound);
    //   //Mix_CloseAudio();
    Mix_Chunk* sound = ResourceManager::getInstance(renderer).getAudio(filename);

    int maxChannels = Mix_AllocateChannels(-1);  // Get the total number of channels
    int availableChannel = -1;

    for (int channel = 0; channel < maxChannels; ++channel) {
        if (Mix_Playing(channel) == 0) {
            availableChannel = channel;
            break;
        }
    }

    if (availableChannel != -1) {
        Mix_PlayChannel(availableChannel, sound, 0);
    } else {
        printf("Failed to play sound effect: %s\n", Mix_GetError());
    }

}


void GameManager::setCamera(const Vector2f& position) {
    camera.setWithCenter(position);

    if ( cameraLimitBox.size.x == 0 && cameraLimitBox.size.y == 0) return;

    if (camera.position.x < cameraLimitBox.position.x) camera.position.x = cameraLimitBox.position.x;
    if (camera.getRight() > cameraLimitBox.getRight()) camera.position.x = cameraLimitBox.getRight()-camera.size.x;
    if (camera.position.y < cameraLimitBox.position.y) camera.position.y = cameraLimitBox.position.y;
    if (camera.getBottom() > cameraLimitBox.getBottom()) camera.position.y = cameraLimitBox.getBottom()-camera.size.y;
}

void GameManager::quit()
{
  SDL_DestroyRenderer(GameManager::renderer);
  SDL_DestroyWindow(GameManager::window);
  
  TTF_Quit();
  Mix_Quit();
  SDL_Quit();

  GameManager::running = false;
}

void GameManager::doFade(std::function<void()> middle) {
    onMiddleFade = middle; 
    transition = 1;
    fadeStartTime = SDL_GetTicks();
}
